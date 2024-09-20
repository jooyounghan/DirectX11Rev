#include "SkeletalMeshObject.h"

#include "GlobalVariable.h"

#include "SkeletalMeshAsset.h"
#include "AssetManager.h"
#include "IGuiModelVisitor.h"

#include "PSOManager.h"
#include "PSOObject.h"

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
