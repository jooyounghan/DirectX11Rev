#pragma once
#include "APickingIDRenderer.h"

class PickingIDWireframeRenderer : public APickingIDRenderer
{
public:
	PickingIDWireframeRenderer(PSOObject* PSOObjectIn);

public:
	virtual void Render(ID3D11DeviceContext* DeviceContextIn, MeshObject* MeshObjectIn) override;
	virtual void Render(ID3D11DeviceContext* DeviceContextIn, ABoundingObject* MeshObjectIn) override;
};
