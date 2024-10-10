#include "StaticMeshObject.h"

#include "GlobalVariable.h"
#include "IGuiModelVisitor.h"

#include "AssetManager.h"
#include "StaticMeshAsset.h"	

#include "PSOManager.h"
#include "PSOObject.h"

#include "EnvironmentActor.h"

using namespace std;

const char* StaticMeshObject::StaticMeshObjectIdentifier = "Static Mesh Object";

StaticMeshObject::StaticMeshObject(MapAsset* MapAssetInstance, std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstanceIn)
	: AMeshObject(MapAssetInstance), StaticMeshAssetInstance(StaticMeshAssetInstanceIn)
{
	static size_t StaticMeshObjectCount = 0;

	StaticMeshObjectCount++;
	ObjectName = StaticMeshObjectIdentifier + to_string(StaticMeshObjectCount);
	AttachableKind = EAttachableObjectKind::STATIC_MESH_KIND;

	MeshObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Static_Solid);
	PickingIDSolidPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Static_ID_Solid);

	if (StaticMeshAssetInstanceIn != nullptr)
	{
		MaterialAssetInstances = StaticMeshAssetInstanceIn->GetDefaultMaterialAssets();
	}
}

StaticMeshObject::~StaticMeshObject()
{
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
	AMeshObject::Render();
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
