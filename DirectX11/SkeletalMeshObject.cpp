#include "SkeletalMeshObject.h"

#include "IGuiModelVisitor.h"
#include "GlobalVariable.h"

#include "AssetManager.h"
#include "SkeletalMeshAsset.h"

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

	MeshObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Skeletal_Solid);
	PickingIDSolidPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Skeletal_ID_Solid);

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
	AMeshObject::Render();
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
