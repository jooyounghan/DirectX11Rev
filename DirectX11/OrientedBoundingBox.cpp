#include "OrientedBoundingBox.h"
#include "GraphicsPipeline.h"
#include "Debugable.h"
#include "PSOObject.h"

using namespace DirectX;



OrientedBoundingBox::OrientedBoundingBox(
	GraphicsPipeline* GraphicsPipelineInstances, 
	const float& RadiusIn, 
	Debugable* DebugObjectIn
)
	: RelativePlaceableObject(
		GraphicsPipelineInstances->GetDevice(),
		GraphicsPipelineInstances->GetDeviceContext()
	), DebugObject(DebugObjectIn)
{
}

bool OrientedBoundingBox::Intersect(const Ray& RayIn, float& DistanceOut)
{
	return false;
}

void OrientedBoundingBox::UpdateObject(const float& DeltaTimeIn)
{
}

void OrientedBoundingBox::Render(PSOObject* PSOObjectIn)
{
}
