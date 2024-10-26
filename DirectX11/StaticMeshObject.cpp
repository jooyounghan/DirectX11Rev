#include "StaticMeshObject.h"

#include "GlobalVariable.h"
#include "IGuiModelVisitor.h"

#include "AssetManager.h"
#include "StaticMeshAsset.h"	

#include "PSOManager.h"
#include "PSOObject.h"

#include "EnvironmentActor.h"

using namespace std;

string StaticMeshObject::StaticMeshObjectKind = "Static Mesh Object";

StaticMeshObject::StaticMeshObject(MapAsset* MapAssetInstance, std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstanceIn)
	: AMeshObject(MapAssetInstance, StaticMeshObject::StaticMeshObjectKind)
{
	static size_t StaticMeshObjectCount = 0;

	StaticMeshObjectCount++;
	ObjectName = StaticMeshObject::StaticMeshObjectKind + to_string(StaticMeshObjectCount);

	MeshObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Static_Solid);
	PickingIDSolidPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Static_ID_Solid);

	SetStaticMeshAssetInstance(StaticMeshAssetInstanceIn);
}

StaticMeshObject::~StaticMeshObject()
{
}

void StaticMeshObject::SetStaticMeshAssetInstance(const std::shared_ptr<StaticMeshAsset>& StaticMeshAssetInstanceIn)
{
	StaticMeshAssetInstance = StaticMeshAssetInstanceIn;
	if (StaticMeshAssetInstanceIn != nullptr)
	{
		MaterialAssetInstances = StaticMeshAssetInstanceIn->GetDefaultMaterialAssets();
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
	AMeshObject::Render();
}

void StaticMeshObject::OnSerializeFromMap(FILE* FileIn)
{
	AMeshObject::OnSerializeFromMap(FileIn);

	// Mesh Asset Name
	AAssetFile::SerializeString(StaticMeshAssetInstance != nullptr ? StaticMeshAssetInstance->GetAssetName() : "", FileIn);

}

void StaticMeshObject::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AMeshObject::OnDeserializeToMap(FileIn, AssetManagerIn);

	string MeshAssetName;
	AAssetFile::DeserializeString(MeshAssetName, FileIn);

	StaticMeshAssetInstance = AssetManagerIn->GetManagingStaticMesh(MeshAssetName);
}
