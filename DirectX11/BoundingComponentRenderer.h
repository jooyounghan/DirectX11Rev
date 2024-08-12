#pragma once
#include "ARenderer.h"

class ABoundingComponent;

class BoundingComponentRenderer : public ARenderer
{
public:
	BoundingComponentRenderer(PSOObject* PSOObjectIn);

public:
	virtual void PresetRendering(Camera* CameraIn, Map* MapIn) override;
	virtual void ResetRendering() override;

public:
	virtual void Render(MeshObject* MeshObjectIn) override;
	virtual void Render(ABoundingComponent* MeshObjectIn) override;
};

