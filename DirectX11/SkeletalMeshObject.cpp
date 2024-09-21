#include "SkeletalMeshObject.h"

#include "GlobalVariable.h"

#include "SkeletalMeshAsset.h"
#include "AssetManager.h"
#include "IGuiModelVisitor.h"

#include "PSOManager.h"
#include "PSOObject.h"

#include "MapAsset.h"
#include "Camera.h"

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
	if (CurrentCamera != nullptr && SkeletalMeshAssetInstance != nullptr)
	{
		ID3D11RenderTargetView* RTVs[]{ CurrentCamera->GetSDRSceneRTV() };
		const std::vector<ID3D11Buffer*> VertexBuffers = SkeletalMeshAssetInstance->GetVertexBuffers();
		const std::vector<UINT> Strides = SkeletalMeshAssetInstance->GetStrides();
		const std::vector<UINT> Offsets = SkeletalMeshAssetInstance->GetOffsets();
		ID3D11Buffer* IndexBuffer = SkeletalMeshAssetInstance->GetIndexBuffer();
		const DXGI_FORMAT IndexFormat = SkeletalMeshAssetInstance->GetIndexFormat();
		const UINT IndexCount = SkeletalMeshAssetInstance->GetIndexCount();
		ID3D11Buffer* VSConstBuffers[] = { CurrentCamera->ViewProjBuffer.GetBuffer(), TransformationBuffer.GetBuffer() };
		ID3D11Buffer* PSConstBuffers[] = { PickingIDBufferCached };
		const D3D11_VIEWPORT* Viewport = &CurrentCamera->GetViewport();
		ID3D11DepthStencilView* DSV = CurrentCamera->GetSceneDSV();


#ifdef DIRECT_RENDER
		SPSOArgument CommonPSOArgument;

		CommonPSOArgument.VertexBuffers = SkeletalMeshAssetInstance->GetVertexBuffers();
		CommonPSOArgument.Strides = SkeletalMeshAssetInstance->GetStrides();
		CommonPSOArgument.Offsets = SkeletalMeshAssetInstance->GetOffsets();
		CommonPSOArgument.IndexBuffer = SkeletalMeshAssetInstance->GetIndexBuffer();
		CommonPSOArgument.IndexFormat = SkeletalMeshAssetInstance->GetIndexFormat();
		CommonPSOArgument.IndexCount = SkeletalMeshAssetInstance->GetIndexCount();
		CommonPSOArgument.VSConstantBuffers = vector<ID3D11Buffer*>{ CurrentCamera->ViewProjBuffer.GetBuffer(), TransformationBuffer.GetBuffer() };
		CommonPSOArgument.PSConstantBuffers = vector<ID3D11Buffer*>{ PickingIDBufferCached };
		CommonPSOArgument.RTVs = vector<ID3D11RenderTargetView*>{ CurrentCamera->GetSDRSceneRTV() };
		CommonPSOArgument.DSV = CurrentCamera->GetSceneDSV();
		CommonPSOArgument.Viewport = &CurrentCamera->GetViewport();

		App::GPSOManager->AddRenderCommand(SkeletalMeshObjectPSOCached, CommonPSOArgument);
		App::GPSOManager->AddRenderCommand(PickingIDSolidSkeletalPSOCached, CommonPSOArgument);

#else
#pragma region SkeletalMeshObjectPSOCached
		SkeletalMeshObjectPSOCached->SetPipelineStateObject(1, RTVs, Viewport, DSV);
		SkeletalMeshObjectPSOCached->SetVSConstantBuffers(0, 2, VSConstBuffers);
		DeviceContextCached->IASetVertexBuffers(0, static_cast<UINT>(VertexBuffers.size()),
			VertexBuffers.data(),
			Strides.data(),
			Offsets.data()
		);
		DeviceContextCached->IASetIndexBuffer(IndexBuffer, IndexFormat, 0);

#ifdef _DEBUG
		SkeletalMeshObjectPSOCached->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextCached->DrawIndexed(static_cast<UINT>(IndexCount), 0, 0);

		SkeletalMeshObjectPSOCached->ResetVSConstantBuffers(0, 2);
#pragma endregion

#pragma region PickingIDSolidSkeletalPSOCached


		PickingIDSolidSkeletalPSOCached->SetPipelineStateObject(1, RTVs, &CurrentCamera->GetViewport(), CurrentCamera->GetSceneDSV());

		PickingIDSolidSkeletalPSOCached->SetVSConstantBuffers(0, 2, VSConstBuffers);
		PickingIDSolidSkeletalPSOCached->SetPSConstantBuffers(0, 1, PSConstBuffers);

		DeviceContextCached->IASetVertexBuffers(0, static_cast<UINT>(VertexBuffers.size()),
			VertexBuffers.data(),
			Strides.data(),
			Offsets.data()
		);
		DeviceContextCached->IASetIndexBuffer(IndexBuffer, IndexFormat, 0);


#ifdef _DEBUG
		PickingIDSolidSkeletalPSOCached->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextCached->DrawIndexed(static_cast<UINT>(IndexCount), 0, 0);

		PickingIDSolidSkeletalPSOCached->ResetVSConstantBuffers(0, 2);
#pragma endregion
#endif

	}
}

void SkeletalMeshObject::OnSerializeFromMap(FILE* FileIn)
{
	AObject::OnSerializeFromMap(FileIn);

	// Mesh Asset Name
	if (SkeletalMeshAssetInstance != nullptr)
	{
		const string& AssetName = SkeletalMeshAssetInstance->GetAssetName();
		SerializeString(AssetName, FileIn);
	}
	else
	{
		const string DummyAssetName = "";
		SerializeString(DummyAssetName, FileIn);
	}
}

void SkeletalMeshObject::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserializeToMap(FileIn, AssetManagerIn);

	string MeshAssetName;
	DeserializeString(MeshAssetName, FileIn);
	SkeletalMeshAssetInstance = AssetManagerIn->GetManagingSkeletalMesh(MeshAssetName);
}
