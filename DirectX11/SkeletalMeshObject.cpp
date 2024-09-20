#include "SkeletalMeshObject.h"

#include "GlobalVariable.h"

#include "SkeletalMeshAsset.h"
#include "AssetManager.h"
#include "IGuiModelVisitor.h"

#include "PSOManager.h"
#include "MeshObjectPSO.h"
#include "PickingIDSolidPSO.h"
using namespace std;

const char* SkeletalMeshObject::SkeletalMeshObjectIdentifier = "Skeletal Mesh Object";

SkeletalMeshObject::SkeletalMeshObject(std::shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetInstanceIn)
	: AMeshObject(), SkeletalMeshAssetInstance(SkeletalMeshAssetInstanceIn)
{
	static size_t SkeletalMeshObjectCount = 0;

	SkeletalMeshObjectCount++;
	ObjectName = SkeletalMeshObjectIdentifier + to_string(SkeletalMeshObjectCount);
	AttachableKind = EAttachableObjectKind::SKELETAL_MESH_KIND;

	App::GPSOManager->AddObjectToPSO<SkeletalMeshObject, MeshObjectPSO<SkeletalMeshObject>>(EPSOType::Skeletal_Solid, this);
	App::GPSOManager->AddObjectToPSO<SkeletalMeshObject, PickingIDSolidPSO<SkeletalMeshObject>>(EPSOType::Skeletal_ID_Solid, this);
}

SkeletalMeshObject::~SkeletalMeshObject()
{
	App::GPSOManager->RemoveObjectFromPSO<SkeletalMeshObject, MeshObjectPSO<SkeletalMeshObject>>(EPSOType::Skeletal_Solid, this);
	App::GPSOManager->RemoveObjectFromPSO<SkeletalMeshObject, PickingIDSolidPSO<SkeletalMeshObject>>(EPSOType::Skeletal_ID_Solid, this);
}

AMeshAsset* SkeletalMeshObject::GetMeshAssetInstance()
{
	return SkeletalMeshAssetInstance.get();
}

void SkeletalMeshObject::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
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
