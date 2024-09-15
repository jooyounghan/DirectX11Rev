#include "MeshObject.h"
#include "GraphicsPipeline.h"

#include "AssetManager.h"
#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"

#include "APSOObject.h"
#include "IGuiModelVisitor.h"

using namespace std;

const char* MeshObject::MeshObjectIdentifier = "Mesh Object";
size_t MeshObject::MeshObjectCount = 0;

MeshObject::MeshObject(std::shared_ptr<AMeshAsset> MeshAssetInstanceIn)
	
{
	MeshObjectCount++;
	ObjectName = MeshObjectIdentifier + to_string(MeshObjectCount);
	AttachableKind = EAttachableObjectKind::MESH_KIND;
}

MeshObject::~MeshObject()
{
}

void MeshObject::Update(const float& DeltaTimeIn)
{
	AAttachableObject::Update(DeltaTimeIn);
}

void MeshObject::AcceptGui(IGuiModelVisitor* GuiVisitor)
{

}

void MeshObject::OnSerializeFromMap(FILE* FileIn)
{

}

void MeshObject::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{

}
