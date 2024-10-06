#include "SkeletalMeshObject.h"

#include "GlobalVariable.h"

#include "SkeletalMeshAsset.h"
#include "AssetManager.h"
#include "IGuiModelVisitor.h"

#include "PSOManager.h"
#include "PSOObject.h"

#include "MapAsset.h"
#include "Camera.h"
#include "EnvironmentActor.h"
#include "DDSTextureAsset.h"

using namespace std;

const char* SkeletalMeshObject::SkeletalMeshObjectIdentifier = "Skeletal Mesh Object";

SkeletalMeshObject::SkeletalMeshObject(MapAsset* MapAssetInstance, std::shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetInstanceIn)
	: AMeshObject(MapAssetInstance), SkeletalMeshAssetInstance(SkeletalMeshAssetInstanceIn)
{
	static size_t SkeletalMeshObjectCount = 0;

	SkeletalMeshObjectCount++;
	ObjectName = SkeletalMeshObjectIdentifier + to_string(SkeletalMeshObjectCount);
	AttachableKind = EAttachableObjectKind::SKELETAL_MESH_KIND;

	SkeletalMeshObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Skeletal_Solid);
	PickingIDSolidSkeletalPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Skeletal_ID_Solid);

	if (SkeletalMeshAssetInstanceIn != nullptr)
	{
		MaterialAssetInstances = SkeletalMeshAssetInstanceIn->GetDefaultMaterialAssets();
	}
}

SkeletalMeshObject::~SkeletalMeshObject()
{
}

AMeshAsset* SkeletalMeshObject::GetMeshAssetInstance()
{
	return SkeletalMeshAssetInstance.get();
}

void SkeletalMeshObject::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void SkeletalMeshObject::Render()
{
	AAttachableObject::Render();

	Camera* CurrentCamera = MapAssetCached->GetCurrentCamera();
	EnvironmentActor* CurrentEnvironment = MapAssetCached->GetEnvironmentActorInstance();
	if (CurrentCamera != nullptr && SkeletalMeshAssetInstance != nullptr && CurrentEnvironment != nullptr)
	{
		const size_t LODLevel = GetLODLevel(
			CurrentCamera->GetAbsolutePosition(),
			*CurrentCamera->GetPointerFarZ(),
			SkeletalMeshAssetInstance->GetLODCount(),
			3
		);

		vector<ID3D11RenderTargetView*> RTVs;
		const D3D11_VIEWPORT* Viewport = &CurrentCamera->GetViewport();
		ID3D11DepthStencilView* DSV = CurrentCamera->GetSceneDSV();
		const std::vector<ID3D11Buffer*> VertexBuffers = SkeletalMeshAssetInstance->GetVertexBuffers(LODLevel);
		const std::vector<UINT> Strides = SkeletalMeshAssetInstance->GetStrides();
		const std::vector<UINT> Offsets = SkeletalMeshAssetInstance->GetOffsets();
		ID3D11Buffer* IndexBuffer = SkeletalMeshAssetInstance->GetIndexBuffer(LODLevel);
		const DXGI_FORMAT IndexFormat = SkeletalMeshAssetInstance->GetIndexFormat();
		const UINT IndexCount = SkeletalMeshAssetInstance->GetIndexCount(LODLevel);
		vector<ID3D11Buffer*> VSConstBuffers;
		vector<ID3D11Buffer*> PSConstBuffers;
		vector<ID3D11ShaderResourceView*> PSSRVs;

		RTVs.push_back(CurrentCamera->GetSDRSceneRTV());

		VSConstBuffers.push_back(CurrentCamera->ViewProjBuffer.GetBuffer());
		VSConstBuffers.push_back(TransformationBuffer.GetBuffer());
		PSConstBuffers.push_back(PickingIDBufferCached);

		DDSTextureAsset* SpecularDDS = CurrentEnvironment->GetEnvironmentSpecularDDSTextureAsset();
		DDSTextureAsset* DiffuseDDS = CurrentEnvironment->GetEnvironmentDiffuseDDSTextureAsset();
		DDSTextureAsset* BRDFDDS = CurrentEnvironment->GetEnvironmentBRDFDDSTextureAsset();
	
		PSSRVs.push_back(SpecularDDS != nullptr ? SpecularDDS->GetSRV() : nullptr);
		PSSRVs.push_back(DiffuseDDS != nullptr ? DiffuseDDS->GetSRV() : nullptr);
		PSSRVs.push_back(BRDFDDS != nullptr ? BRDFDDS->GetSRV() : nullptr);


#pragma region SkeletalMeshObjectPSOCached
		SkeletalMeshObjectPSOCached->SetPipelineStateObject(RTVs.size(), RTVs.data(), Viewport, DSV);
		DeviceContextCached->IASetIndexBuffer(IndexBuffer, IndexFormat, 0);
		DeviceContextCached->IASetVertexBuffers(0, static_cast<UINT>(VertexBuffers.size()),
			VertexBuffers.data(),
			Strides.data(),
			Offsets.data()
		);

		SkeletalMeshObjectPSOCached->SetVSConstantBuffers(0, VSConstBuffers.size(), VSConstBuffers.data());
		PickingIDSolidSkeletalPSOCached->SetPSShaderResourceViews(0, PSConstBuffers.size(), PSSRVs.data());
#ifdef _DEBUG
		SkeletalMeshObjectPSOCached->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextCached->DrawIndexed(static_cast<UINT>(IndexCount), 0, 0);
		Performance::GTotalIndexCount += IndexCount;

		SkeletalMeshObjectPSOCached->ResetVSConstantBuffers(0, VSConstBuffers.size());
		PickingIDSolidSkeletalPSOCached->ResetPSShaderResourceViews(0, PSConstBuffers.size());
#pragma endregion

#pragma region PickingIDSolidSkeletalPSOCached

		PickingIDSolidSkeletalPSOCached->SetPipelineStateObject(RTVs.size(), RTVs.data(), &CurrentCamera->GetViewport(), CurrentCamera->GetSceneDSV());

		DeviceContextCached->IASetVertexBuffers(0, static_cast<UINT>(VertexBuffers.size()),
			VertexBuffers.data(),
			Strides.data(),
			Offsets.data()
		);
		DeviceContextCached->IASetIndexBuffer(IndexBuffer, IndexFormat, 0);

		PickingIDSolidSkeletalPSOCached->SetVSConstantBuffers(0, VSConstBuffers.size(), VSConstBuffers.data());
		PickingIDSolidSkeletalPSOCached->SetPSConstantBuffers(0, PSConstBuffers.size(), PSConstBuffers.data());


#ifdef _DEBUG
		PickingIDSolidSkeletalPSOCached->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextCached->DrawIndexed(static_cast<UINT>(IndexCount), 0, 0);

		PickingIDSolidSkeletalPSOCached->ResetVSConstantBuffers(0, VSConstBuffers.size());
		PickingIDSolidSkeletalPSOCached->ResetPSConstantBuffers(0, PSConstBuffers.size());
#pragma endregion
	}
}

void SkeletalMeshObject::OnSerializeFromMap(FILE* FileIn)
{
	AMeshObject::OnSerializeFromMap(FileIn);

	// Mesh Asset Name
	AAssetFile::SerializeString(SkeletalMeshAssetInstance != nullptr ? SkeletalMeshAssetInstance->GetAssetName() : "", FileIn);
}

void SkeletalMeshObject::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AMeshObject::OnDeserializeToMap(FileIn, AssetManagerIn);

	string MeshAssetName;
	AAssetFile::DeserializeString(MeshAssetName, FileIn);

	SkeletalMeshAssetInstance = AssetManagerIn->GetManagingSkeletalMesh(MeshAssetName);
}
