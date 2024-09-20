#include "StaticMeshObject.h"

#include "GlobalVariable.h"

#include "StaticMeshAsset.h"	
#include "AssetManager.h"
#include "IGuiModelVisitor.h"

#include "PSOManager.h"
#include "MeshObjectPSO.h"
#include "PickingIDSolidPSO.h"

using namespace std;

const char* StaticMeshObject::StaticMeshObjectIdentifier = "Static Mesh Object";

StaticMeshObject::StaticMeshObject(std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstanceIn)
	: AMeshObject(), StaticMeshAssetInstance(StaticMeshAssetInstanceIn)
{
	static size_t StaticMeshObjectCount = 0;

	StaticMeshObjectCount++;
	ObjectName = StaticMeshObjectIdentifier + to_string(StaticMeshObjectCount);
	AttachableKind = EAttachableObjectKind::STATIC_MESH_KIND;

	App::GPSOManager->AddObjectToPSO<StaticMeshObject, MeshObjectPSO<StaticMeshObject>>(EPSOType::Static_Solid, this);
	App::GPSOManager->AddObjectToPSO<StaticMeshObject, PickingIDSolidPSO<StaticMeshObject>>(EPSOType::Static_ID_Solid, this);
}

StaticMeshObject::~StaticMeshObject()
{
	App::GPSOManager->RemoveObjectFromPSO<StaticMeshObject, MeshObjectPSO<StaticMeshObject>>(EPSOType::Static_Solid, this);
	App::GPSOManager->RemoveObjectFromPSO<StaticMeshObject, PickingIDSolidPSO<StaticMeshObject>>(EPSOType::Static_ID_Solid, this);
}

AMeshAsset* StaticMeshObject::GetMeshAssetInstance() 
{
	return StaticMeshAssetInstance.get();
}

void StaticMeshObject::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
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
