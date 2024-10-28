#include "SkeletalMeshObject.h"

#include "IGuiModelVisitor.h"
#include "GlobalVariable.h"

#include "AssetManager.h"
#include "SkeletalMeshAsset.h"

#include "PSOManager.h"
#include "PSOObject.h"

#include "MapAsset.h"
#include "ACamera.h"

#include "EnvironmentActor.h"


using namespace std;

string SkeletalMeshObject::SkeletalMeshObjectKind = "Skeletal Mesh Object";

SkeletalMeshObject::SkeletalMeshObject(MapAsset* MapAssetInstance, std::shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetInstanceIn)
	: AMeshObject(MapAssetInstance, SkeletalMeshObject::SkeletalMeshObjectKind)
{
	static size_t SkeletalMeshObjectCount = 0;

	SkeletalMeshObjectCount++;
	ObjectName = SkeletalMeshObject::SkeletalMeshObjectKind + to_string(SkeletalMeshObjectCount);

	MeshObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Skeletal_Solid);
	PickingIDSolidPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Skeletal_ID_Solid);

	SetSkeletalMeshAssetInstance(SkeletalMeshAssetInstanceIn);
}

SkeletalMeshObject::~SkeletalMeshObject()
{
}

void SkeletalMeshObject::SetSkeletalMeshAssetInstance(const std::shared_ptr<SkeletalMeshAsset>& SkeletalMeshAssetInstanceIn)
{
	SkeletalMeshAssetInstance = SkeletalMeshAssetInstanceIn;
	if (SkeletalMeshAssetInstanceIn != nullptr)
	{
		MaterialAssetInstances = SkeletalMeshAssetInstanceIn->GetDefaultMaterialAssets();
	}
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
