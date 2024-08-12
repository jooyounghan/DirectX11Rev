#pragma once
#include "ARenderer.h"

class MeshObject;

class MeshObjectRenderer : public ARenderer
{
public:
	MeshObjectRenderer(PSOObject* PSOObjectIn);

public:
	virtual void PresetRendering(Camera* CameraIn, Map* MapIn) override;
	virtual void ResetRendering() override;

public:
	virtual void Render(MeshObject* MeshObjectIn) override;
	virtual void Render(ABoundingComponent* MeshObjectIn) override;
};

