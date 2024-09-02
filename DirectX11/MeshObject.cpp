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

MeshObject::MeshObject() 
	: AAttachableObject()
{
	InitMeshObject();
}

MeshObject::MeshObject(std::shared_ptr<AMeshAsset> MeshAssetInstanceIn)
	: AAttachableObject(), MeshAssetInstance(MeshAssetInstanceIn)
{
	InitMeshObject();
}

MeshObject::~MeshObject()
{
}

void MeshObject::InitMeshObject()
{
	MeshObjectCount++;
	ObjectName = MeshObjectIdentifier + to_string(MeshObjectCount);
	AttachableKind = EAttachableObjectKind::MESH_KIND;
}

void MeshObject::Update(const float& DeltaTimeIn)
{
	AAttachableObject::Update(DeltaTimeIn);
}

void MeshObject::AcceptRenderer(ARenderer* Renderer)
{
	AAttachableObject::AcceptRenderer(Renderer);
	Renderer->Render(this);
}

void MeshObject::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void MeshObject::OnSerialize(FILE* FileIn)
{
	AObject::OnSerialize(FileIn);

	// Mesh Asset Name
	if (MeshAssetInstance != nullptr)
	{
		fwrite(&MeshAssetInstance->GetAssetType(), sizeof(EAssetType), 1, FileIn);

		size_t MeshAssetNameCount = 0;
		const string& AssetName = MeshAssetInstance->GetAssetName();
		MeshAssetNameCount = AssetName.size();
		fwrite(&MeshAssetNameCount, sizeof(size_t), 1, FileIn);
		fwrite(AssetName.c_str(), sizeof(char), MeshAssetNameCount, FileIn);
	}
	else
	{
		EAssetType NoneType = EAssetType::None;
		fwrite(&NoneType, sizeof(EAssetType), 1, FileIn);
	}
}

void MeshObject::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserialize(FileIn, AssetManagerIn);

	EAssetType MeshAssetType;
	fread(&MeshAssetType, sizeof(EAssetType), 1, FileIn);

	if (MeshAssetType != EAssetType::None)
	{
		size_t MeshAssetNameCount;
		fread(&MeshAssetNameCount, sizeof(size_t), 1, FileIn);

		string MeshAssetName;
		MeshAssetName.resize(MeshAssetNameCount);
		fread(MeshAssetName.data(), sizeof(char), MeshAssetNameCount, FileIn);

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
