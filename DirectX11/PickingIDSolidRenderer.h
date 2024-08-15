#pragma once
#include "APickingIDRenderer.h"

class PickingIDSolidRenderer : public APickingIDRenderer
{
public:
	PickingIDSolidRenderer(PSOObject* PSOObjectIn);

public:
	virtual void Render(ID3D11DeviceContext* DeviceContextIn, MeshObject* MeshObjectIn) override;
	virtual void Render(ID3D11DeviceContext* DeviceContextIn, ABoundingComponent* MeshObjectIn) override;
};
