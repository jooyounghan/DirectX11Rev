#pragma once
#include "ARenderer.h"

class APickingIDRenderer : public ARenderer
{
public:
	APickingIDRenderer(PSOObject* PSOObjectIn);

public:
	virtual void PresetRendering(
		const unsigned int& RTVCountIn,
		ID3D11RenderTargetView** RTVsIn,
		D3D11_VIEWPORT* ViewportIn,
		ID3D11DepthStencilView* DSVIn,
		Camera* CameraIn,
		Map* MapIn
	) override final;
	virtual void ResetRendering() override final;

public:
	virtual void Render(ID3D11DeviceContext* DeviceContextIn, MeshObject* MeshObjectIn) = 0;
	virtual void Render(ID3D11DeviceContext* DeviceContextIn, ABoundingComponent* MeshObjectIn) = 0;
};

