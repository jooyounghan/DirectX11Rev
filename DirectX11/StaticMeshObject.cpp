#include "StaticMeshObject.h"

#include "GlobalVariable.h"

#include "StaticMeshAsset.h"	
#include "AssetManager.h"
#include "IGuiModelVisitor.h"

#include "PSOManager.h"
#include "PSOObject.h"

#include "MapAsset.h"
#include "Camera.h"

using namespace std;

const char* StaticMeshObject::StaticMeshObjectIdentifier = "Static Mesh Object";

StaticMeshObject::StaticMeshObject(MapAsset* MapAssetInstance, std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstanceIn)
	: AMeshObject(MapAssetInstance), StaticMeshAssetInstance(StaticMeshAssetInstanceIn)
{
	static size_t StaticMeshObjectCount = 0;

	StaticMeshObjectCount++;
	ObjectName = StaticMeshObjectIdentifier + to_string(StaticMeshObjectCount);
	AttachableKind = EAttachableObjectKind::STATIC_MESH_KIND;

	StaticMeshObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Static_Solid);
	PickingIDSolidStaticPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Static_ID_Solid);

	if (StaticMeshAssetInstanceIn != nullptr)
	{
		MaterialAssetInstances = StaticMeshAssetInstanceIn->GetDefaultMaterialAssets();
	}
}

StaticMeshObject::~StaticMeshObject()
{
}

void StaticMeshObject::SetStaticMeshAssetInstance(std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstanceIn)
{
	if (StaticMeshAssetInstanceIn != nullptr)
	{
		StaticMeshAssetInstance = StaticMeshAssetInstanceIn;
		SetMaterialAssetInstances(StaticMeshAssetInstanceIn->GetDefaultMaterialAssets());
	}
}

AMeshAsset* StaticMeshObject::GetMeshAssetInstance()
{
	return StaticMeshAssetInstance.get();
}

void StaticMeshObject::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void StaticMeshObject::Render()
{
	AAttachableObject::Render();

	Camera* CurrentCamera = MapAssetCached->GetCurrentCamera();
	if (CurrentCamera != nullptr && StaticMeshAssetInstance != nullptr)
	{
		const size_t LODLevel = 0;

		ID3D11RenderTargetView* RTVs[]{ CurrentCamera->GetSDRSceneRTV() };
		const std::vector<ID3D11Buffer*> VertexBuffers = StaticMeshAssetInstance->GetVertexBuffers(LODLevel);
		const std::vector<UINT> Strides = StaticMeshAssetInstance->GetStrides();
		const std::vector<UINT> Offsets = StaticMeshAssetInstance->GetOffsets();
		ID3D11Buffer* VSConstBuffers[] = { CurrentCamera->ViewProjBuffer.GetBuffer(), TransformationBuffer.GetBuffer() };
		ID3D11Buffer* PSConstBuffers[] = { PickingIDBufferCached };

#pragma region StaticMeshObjectPSOCached
		StaticMeshObjectPSOCached->SetPipelineStateObject(1, RTVs, &CurrentCamera->GetViewport(), CurrentCamera->GetSceneDSV());
		StaticMeshObjectPSOCached->SetVSConstantBuffers(0, 2, VSConstBuffers);

		DeviceContextCached->IASetVertexBuffers(0, static_cast<UINT>(VertexBuffers.size()),
			VertexBuffers.data(),
			Strides.data(),
			Offsets.data()
		);
		DeviceContextCached->IASetIndexBuffer(StaticMeshAssetInstance->GetIndexBuffer(LODLevel), StaticMeshAssetInstance->GetIndexFormat(), 0);

#ifdef _DEBUG
		StaticMeshObjectPSOCached->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextCached->DrawIndexed(static_cast<UINT>(StaticMeshAssetInstance->GetIndexCount(LODLevel)), 0, 0);

		StaticMeshObjectPSOCached->ResetVSConstantBuffers(0, 2);
#pragma endregion

#pragma region PickingIDSolidStaticPSOCached


		PickingIDSolidStaticPSOCached->SetPipelineStateObject(1, RTVs, &CurrentCamera->GetViewport(), CurrentCamera->GetSceneDSV());

		PickingIDSolidStaticPSOCached->SetVSConstantBuffers(0, 2, VSConstBuffers);
		PickingIDSolidStaticPSOCached->SetPSConstantBuffers(0, 1, PSConstBuffers);

		DeviceContextCached->IASetVertexBuffers(0, static_cast<UINT>(VertexBuffers.size()),
			VertexBuffers.data(),
			Strides.data(),
			Offsets.data()
		);
		DeviceContextCached->IASetIndexBuffer(StaticMeshAssetInstance->GetIndexBuffer(LODLevel), StaticMeshAssetInstance->GetIndexFormat(), 0);


#ifdef _DEBUG
		PickingIDSolidStaticPSOCached->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextCached->DrawIndexed(static_cast<UINT>(StaticMeshAssetInstance->GetIndexCount(LODLevel)), 0, 0);

		PickingIDSolidStaticPSOCached->ResetVSConstantBuffers(0, 2);
#pragma endregion
	}
}

void StaticMeshObject::OnSerializeFromMap(FILE* FileIn)
{
	AMeshObject::OnSerializeFromMap(FileIn);

	// Mesh Asset Name
	if (StaticMeshAssetInstance != nullptr)
	{
		const string& AssetName = StaticMeshAssetInstance->GetAssetName();
		AAssetFile::SerializeString(AssetName, FileIn);
	}
	else
	{
		const string DummyAssetName = "";
		AAssetFile::SerializeString(DummyAssetName, FileIn);
	}
}

void StaticMeshObject::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserializeToMap(FileIn, AssetManagerIn);

	string MeshAssetName;
	AAssetFile::DeserializeString(MeshAssetName, FileIn);
	StaticMeshAssetInstance = AssetManagerIn->GetManagingStaticMesh(MeshAssetName);
}
