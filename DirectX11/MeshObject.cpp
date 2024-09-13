#include "MeshObject.h"
#include "GraphicsPipeline.h"

#include "AssetManager.h"
#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"

#include "PSOObject.h"
#include "IGuiModelVisitor.h"
#include "ARenderer.h"

using namespace std;

const char* MeshObject::MeshObjectIdentifier = "Mesh Object";
size_t MeshObject::MeshObjectCount = 0;

MeshObject::MeshObject(std::shared_ptr<AMeshAsset> MeshAssetInstanceIn)
	: AAttachableObject(), MeshAssetInstance(MeshAssetInstanceIn)
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

void MeshObject::AcceptRenderer(ARenderer* Renderer)
{
	Renderer->Render(this);
	AcceptChildrenRenderer(Renderer);
}

void MeshObject::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void MeshObject::OnSerializeFromMap(FILE* FileIn)
{
	AObject::OnSerializeFromMap(FileIn);

	// Mesh Asset Name
	if (MeshAssetInstance != nullptr)
	{
		fwrite(&MeshAssetInstance->GetAssetType(), sizeof(EAssetType), 1, FileIn);
		const string& AssetName = MeshAssetInstance->GetAssetName();
		SerializeString(AssetName, FileIn);
	}
	else
	{
		EAssetType NoneType = EAssetType::None;
		fwrite(&NoneType, sizeof(EAssetType), 1, FileIn);
	}
}

void MeshObject::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserializeToMap(FileIn, AssetManagerIn);

	EAssetType MeshAssetType;
	fread(&MeshAssetType, sizeof(EAssetType), 1, FileIn);

	if (MeshAssetType != EAssetType::None)
	{
		string MeshAssetName;
		DeserializeString(MeshAssetName, FileIn);

		if (MeshAssetType == EAssetType::StaticMesh)
		{
			MeshAssetInstance = AssetManagerIn->GetManagingStaticMesh(MeshAssetName);
		}
		else if (MeshAssetType == EAssetType::SkeletalMesh)
		{
			MeshAssetInstance = AssetManagerIn->GetManagingSkeletalMesh(MeshAssetName);
		}
		else;
	}

}
