#pragma once
#include "ARenderer.h"

class APlaceableObject;

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
		MapAsset* MapIn
	) override final;
	virtual void ResetRendering() override final;

public:
	virtual void Render(EnvironmentActor* EnvironmentActorIn) = 0;
	virtual void Render(MeshObject* MeshObjectIn) = 0;
	virtual void Render(ABoundingObject* MeshObjectIn) = 0;
};

