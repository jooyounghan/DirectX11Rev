#pragma once
#include "ARenderer.h"

class ABoundingObject;

class BoundingObjectRenderer : public ARenderer
{
public:
	BoundingObjectRenderer(PSOObject* PSOObjectIn);

public:
	virtual void PresetRendering(
		const unsigned int& RTVCountIn,
		ID3D11RenderTargetView** RTVsIn, 
		D3D11_VIEWPORT* ViewportIn, 
		ID3D11DepthStencilView* DSVIn,
		Camera* CameraIn, 
		MapAsset* MapIn
	) override;
	virtual void ResetRendering() override;

public:
	virtual void Render(ID3D11DeviceContext* DeviceContextIn, MeshObject* MeshObjectIn) override;
	virtual void Render(ID3D11DeviceContext* DeviceContextIn, ABoundingObject* MeshObjectIn) override;
};
