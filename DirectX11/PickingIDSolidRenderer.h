#pragma once
#include "APickingIDRenderer.h"

class PickingIDSolidRenderer : public APickingIDRenderer
{
public:
	PickingIDSolidRenderer(PSOObject* PSOObjectIn);

public:
	virtual void Render(MeshObject* MeshObjectIn) override;
	virtual void Render(ABoundingObject* MeshObjectIn) override;
};
