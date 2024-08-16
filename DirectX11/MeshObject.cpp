#include "MeshObject.h"
#include "GraphicsPipeline.h"
#include "AMeshAsset.h"
#include "PSOObject.h"
#include "IGuiModelVisitor.h"
#include "ARenderer.h"

using namespace std;

MeshObject::MeshObject(
	GraphicsPipeline* GraphicsPipelineInstances, 
	AMeshAsset* MeshAssetInstanceIn
)
	: AttachableObject(
		GraphicsPipelineInstances->GetDevice(),
		GraphicsPipelineInstances->GetDeviceContext()
	), MeshAssetInstance(MeshAssetInstanceIn)
{
	static size_t MeshObjectCount = 0;
	MeshObjectCount++;
	ObjectName = "Mesh Object " + to_string(MeshObjectCount);
}

MeshObject::~MeshObject()
{
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
