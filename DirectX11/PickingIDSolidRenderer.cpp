#include "PickingIDSolidRenderer.h"

#include "MeshObject.h"
#include "AMeshAsset.h"

#include "APSOObject.h"

using namespace std;

PickingIDSolidRenderer::PickingIDSolidRenderer(APSOObject* PSOObjectIn)
	: APickingIDRenderer(PSOObjectIn)
{
}

void PickingIDSolidRenderer::Render(EnvironmentActor* EnvironmentActorIn)
{
}

void PickingIDSolidRenderer::Render(MeshObject* MeshObjectIn)
{

}

void PickingIDSolidRenderer::Render(ABoundingObject* MeshObjectIn)
{
	// Do Nothing
}