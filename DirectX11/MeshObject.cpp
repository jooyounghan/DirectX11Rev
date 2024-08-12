#include "MeshObject.h"
#include "GraphicsPipeline.h"
#include "AMeshAsset.h"
#include "PSOObject.h"
#include "IGuiLowLevelVisitor.h"
#include "ARenderer.h"

using namespace std;

MeshObject::MeshObject(
	GraphicsPipeline* GraphicsPipelineInstances, 
	AMeshAsset* MeshAssetInstanceIn
)
	: RelativePlaceableObject(
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
	RelativePlaceableObject::UpdateObject(DeltaTimeIn);
}

void MeshObject::AcceptRenderer(ARenderer* Renderer)
{
	Renderer->Render(this);
}

void MeshObject::AcceptGui(IGuiLowLevelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}
