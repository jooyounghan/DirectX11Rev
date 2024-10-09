#include "SkeletalMeshObject.h"

#include "IGuiModelVisitor.h"
#include "GlobalVariable.h"

#include "AssetManager.h"
#include "SkeletalMeshAsset.h"
#include "BasicTextureAsset.h"
#include "DDSTextureAsset.h"
#include "MaterialAsset.h"

#include "PSOManager.h"
#include "PSOObject.h"

#include "MapAsset.h"
#include "Camera.h"

#include "EnvironmentActor.h"


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
		const vector<ID3D11Buffer*> VertexBuffers = SkeletalMeshAssetInstance->GetVertexBuffers(LODLevel);
		const vector<UINT> Strides = SkeletalMeshAssetInstance->GetStrides();
		const vector<UINT> Offsets = SkeletalMeshAssetInstance->GetOffsets();
		ID3D11Buffer* IndexBuffer = SkeletalMeshAssetInstance->GetIndexBuffer(LODLevel);
		const DXGI_FORMAT IndexFormat = SkeletalMeshAssetInstance->GetIndexFormat();
		const UINT IndexTotalCount = SkeletalMeshAssetInstance->GetIndexCount(LODLevel);

		const vector<UINT> IndexCounts = SkeletalMeshAssetInstance->GetIndexCountsForPart(LODLevel);
		const vector<UINT> IndexOffsets = SkeletalMeshAssetInstance->GetIndexOffsetsForPart(LODLevel);
		const vector<UINT> MaterialIndex = SkeletalMeshAssetInstance->GetMaterialIndex(LODLevel);

		vector<ID3D11Buffer*> VSConstBuffers;
		vector<ID3D11Buffer*> PSConstBuffers;
		vector<ID3D11ShaderResourceView*> EnvironmentSRVs;

		RTVs.push_back(CurrentCamera->GetSDRSceneRTV());

		VSConstBuffers.push_back(CurrentCamera->ViewProjBuffer.GetBuffer());
		VSConstBuffers.push_back(TransformationBuffer.GetBuffer());
		PSConstBuffers.push_back(PickingIDBufferCached);

		DDSTextureAsset* SpecularDDS = CurrentEnvironment->GetEnvironmentSpecularDDSTextureAsset();
		DDSTextureAsset* DiffuseDDS = CurrentEnvironment->GetEnvironmentDiffuseDDSTextureAsset();
		DDSTextureAsset* BRDFDDS = CurrentEnvironment->GetEnvironmentBRDFDDSTextureAsset();
	
		EnvironmentSRVs.push_back(SpecularDDS != nullptr ? SpecularDDS->GetSRV() : nullptr);
		EnvironmentSRVs.push_back(DiffuseDDS != nullptr ? DiffuseDDS->GetSRV() : nullptr);
		EnvironmentSRVs.push_back(BRDFDDS != nullptr ? BRDFDDS->GetSRV() : nullptr);

		DeviceContextCached->IASetIndexBuffer(IndexBuffer, IndexFormat, 0);
		DeviceContextCached->IASetVertexBuffers(0, static_cast<UINT>(VertexBuffers.size()),
			VertexBuffers.data(),
			Strides.data(),
			Offsets.data()
		);

#pragma region SkeletalMeshObjectPSOCached
		SkeletalMeshObjectPSOCached->SetPipelineStateObject(RTVs.size(), RTVs.data(), Viewport, DSV);

		SkeletalMeshObjectPSOCached->SetVSConstantBuffers(0, VSConstBuffers.size(), VSConstBuffers.data());
		SkeletalMeshObjectPSOCached->SetPSShaderResourceViews(0, EnvironmentSRVs.size(), EnvironmentSRVs.data());

		for (size_t PartIdx = 0; PartIdx < MaterialIndex.size(); ++PartIdx)
		{
			vector<ID3D11ShaderResourceView*> MaterialSRVs;

			const shared_ptr<MaterialAsset>& MaterialInstance = MaterialAssetInstances[MaterialIndex[PartIdx]];
			if (MaterialInstance != nullptr)
			{
				BasicTextureAsset* AO = MaterialInstance->GetAmbientOcculusionTextureAsset();
				BasicTextureAsset* Specualr = MaterialInstance->GetSpecularTextureAsset();
				BasicTextureAsset* Diffuse = MaterialInstance->GetDiffuseTextureAsset();
				BasicTextureAsset* Roughness = MaterialInstance->GetRoughnessTextureAsset();
				BasicTextureAsset* Metalic = MaterialInstance->GetMetalicTextureAsset();
				BasicTextureAsset* Normal = MaterialInstance->GetNormalTextureAsset();
				BasicTextureAsset* Height = MaterialInstance->GetHeightTextureAsset();
				BasicTextureAsset* Emissive = MaterialInstance->GetEmissiveTextureAsset();

				MaterialSRVs.push_back(AO != nullptr ? AO->GetSRV() : nullptr);
				MaterialSRVs.push_back(Specualr != nullptr ? Specualr->GetSRV() : nullptr);
				MaterialSRVs.push_back(Diffuse != nullptr ? Diffuse->GetSRV() : nullptr);
				MaterialSRVs.push_back(Roughness != nullptr ? Roughness->GetSRV() : nullptr);
				MaterialSRVs.push_back(Metalic != nullptr ? Metalic->GetSRV() : nullptr);
				MaterialSRVs.push_back(Normal != nullptr ? Normal->GetSRV() : nullptr);
				MaterialSRVs.push_back(Height != nullptr ? Height->GetSRV() : nullptr);
				MaterialSRVs.push_back(Emissive != nullptr ? Emissive->GetSRV() : nullptr);
			}
			else
			{
				MaterialSRVs.insert(MaterialSRVs.end(), { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr });
			}


			SkeletalMeshObjectPSOCached->SetPSShaderResourceViews(EnvironmentSRVs.size(), MaterialSRVs.size(), MaterialSRVs.data());

			SkeletalMeshObjectPSOCached->CheckPipelineValidation();

			DeviceContextCached->DrawIndexed(IndexCounts[PartIdx], IndexOffsets[PartIdx], 0);
			Performance::GTotalIndexCount += IndexTotalCount;

			SkeletalMeshObjectPSOCached->ResetPSShaderResourceViews(EnvironmentSRVs.size(), MaterialSRVs.size());
		}
		SkeletalMeshObjectPSOCached->ResetVSConstantBuffers(0, VSConstBuffers.size());
		SkeletalMeshObjectPSOCached->ResetPSShaderResourceViews(0, EnvironmentSRVs.size());

#pragma endregion

#pragma region PickingIDSolidSkeletalPSOCached
		PickingIDSolidSkeletalPSOCached->SetPipelineStateObject(RTVs.size(), RTVs.data(), &CurrentCamera->GetViewport(), CurrentCamera->GetSceneDSV());

		PickingIDSolidSkeletalPSOCached->SetVSConstantBuffers(0, VSConstBuffers.size(), VSConstBuffers.data());
		PickingIDSolidSkeletalPSOCached->SetPSConstantBuffers(0, PSConstBuffers.size(), PSConstBuffers.data());

		PickingIDSolidSkeletalPSOCached->CheckPipelineValidation();

		DeviceContextCached->DrawIndexed(static_cast<UINT>(IndexTotalCount), 0, 0);

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
