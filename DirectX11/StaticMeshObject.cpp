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

	MeshObjectPSO<StaticMeshObject>* Pso
		= reinterpret_cast<MeshObjectPSO<StaticMeshObject>*>(App::GPSOManager->GetPSOObject(EPSOType::R8G8B8A8_Static_Solid));
	if (Pso != nullptr) Pso->AddMeshObject(this);
	PickingIDSolidPSO<StaticMeshObject>* PsoForID
		= reinterpret_cast<PickingIDSolidPSO<StaticMeshObject>*>(App::GPSOManager->GetPSOObject(EPSOType::R8G8B8A8_Static_ID_Solid));
	if (PsoForID != nullptr) PsoForID->AddMeshObject(this);
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
