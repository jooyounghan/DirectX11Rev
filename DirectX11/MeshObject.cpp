#include "MeshObject.h"
#include "GraphicsPipeline.h"

#include "AssetManager.h"
#include "AMeshAsset.h"

#include "PSOObject.h"
#include "IGuiModelVisitor.h"
#include "ARenderer.h"

using namespace std;

const char* MeshObject::MeshObjectIdentifier = "Mesh Object";
size_t MeshObject::MeshObjectCount = 0;

MeshObject::MeshObject(GraphicsPipeline* GraphicsPipelineInstance) 
	: AttachableObject(GraphicsPipelineInstance)
{
	InitMeshObject();
}

MeshObject::MeshObject(
	GraphicsPipeline* GraphicsPipelineInstance, 
	AMeshAsset* MeshAssetInstanceIn
)
	: AttachableObject(GraphicsPipelineInstance), MeshAssetInstance(MeshAssetInstanceIn)
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
	AttachableKind = EAttachableObjectKind::MeshObjectKind;
}

void MeshObject::UpdateObject(const float& DeltaTimeIn)
{
	AttachableObject::UpdateObject(DeltaTimeIn);
}

void MeshObject::AcceptRenderer(ARenderer* Renderer)
{
	AttachableObject::AcceptRenderer(Renderer);
	Renderer->Render(DeviceContextCached, this);
}

void MeshObject::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void MeshObject::OnSerialize(FILE* FileIn)
{
	AObject::OnSerialize(FileIn);

	// Mesh Asset Name
	size_t MeshAssetNameCount = 0;
	if (MeshAssetInstance != nullptr)
	{
		const string& AssetName = MeshAssetInstance->GetAssetName();
		MeshAssetNameCount = AssetName.size();
		fwrite(&MeshAssetNameCount, sizeof(size_t), 1, FileIn);
		fwrite(AssetName.c_str(), sizeof(char), MeshAssetNameCount, FileIn);
	}
	else
	{
		fwrite(&MeshAssetNameCount, sizeof(size_t), 1, FileIn);
	}
}

void MeshObject::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserialize(FileIn, AssetManagerIn);

	string MeshAssetName;
	size_t MeshAssetNameCount;
	fread(&MeshAssetNameCount, sizeof(size_t), 1, FileIn);
	MeshAssetName.resize(MeshAssetNameCount);
	fread(MeshAssetName.data(), sizeof(char), MeshAssetNameCount, FileIn);

	AAssetFile* AssetFile = AssetManagerIn->GetAsset(MeshAssetName);
	if (AssetFile != nullptr)
	{
		MeshAssetInstance = (AMeshAsset*)AssetFile;
	}
}
