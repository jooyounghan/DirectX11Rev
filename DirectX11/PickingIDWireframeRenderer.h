#pragma once
#include "APickingIDRenderer.h"

class PickingIDWireframeRenderer : public APickingIDRenderer
{
public:
	PickingIDWireframeRenderer(PSOObject* PSOObjectIn);

public:
	virtual void Render(EnvironmentActor* EnvironmentActorIn) override;
	virtual void Render(MeshObject* MeshObjectIn) override;
	virtual void Render(ABoundingObject* MeshObjectIn) override;
};
