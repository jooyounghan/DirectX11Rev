#include "StaticMeshObject.h"

#include "GlobalVariable.h"

#include "StaticMeshAsset.h"	
#include "AssetManager.h"
#include "IGuiModelVisitor.h"

#include "PSOManager.h"

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
}

void StaticMeshObject::OnSerializeFromMap(FILE* FileIn)
{
	AObject::OnSerializeFromMap(FileIn);

	// Mesh Asset Name
	if (StaticMeshAssetInstance != nullptr)
	{
		const string& AssetName = StaticMeshAssetInstance->GetAssetName();
		SerializeString(AssetName, FileIn);
	}
	else
	{
		const string DummyAssetName = "";
		SerializeString(DummyAssetName, FileIn);
	}
}

void StaticMeshObject::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserializeToMap(FileIn, AssetManagerIn);

	string MeshAssetName;
	DeserializeString(MeshAssetName, FileIn);
	StaticMeshAssetInstance = AssetManagerIn->GetManagingStaticMesh(MeshAssetName);
}
