#include "MeshObject.h"
#include "GraphicsPipeline.h"
#include "AMeshAsset.h"
#include "PSOObject.h"
#include "IGuiModelVisitor.h"
#include "ARenderer.h"

using namespace std;

const char* MeshObject::MeshObjectIdentifier = "Mesh Object";
size_t MeshObject::MeshObjectCount = 0;

MeshObject::MeshObject(GraphicsPipeline* GraphicsPipelineInstances) 
	: AttachableObject(
	GraphicsPipelineInstances->GetDevice(),
	GraphicsPipelineInstances->GetDeviceContext()
	)
{
	InitMeshObject();
}

MeshObject::MeshObject(
	GraphicsPipeline* GraphicsPipelineInstances, 
	AMeshAsset* MeshAssetInstanceIn
)
	: AttachableObject(
		GraphicsPipelineInstances->GetDevice(),
		GraphicsPipelineInstances->GetDeviceContext()
	), MeshAssetInstance(MeshAssetInstanceIn)
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
