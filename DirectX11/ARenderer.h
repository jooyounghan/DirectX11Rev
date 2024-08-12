#pragma once
#include "HeaderHelper.h"
#include <list>

class PSOObject;

class MeshObject;
class ABoundingComponent;
class Map;
class Camera;

struct ID3D11RenderTargetView;
struct D3D11_VIEWPORT;
struct ID3D11DepthStencilView;
struct ID3D11DeviceContext;

class ARenderer
{
public:
	ARenderer(PSOObject* PSOObjectIn) : PSOObjectCached(PSOObjectIn) {};

protected:
	PSOObject* PSOObjectCached = nullptr;

public:
	virtual void PresetRendering(
		const unsigned int& RTVCountIn,
		ID3D11RenderTargetView** RTVsIn, 
		D3D11_VIEWPORT* ViewportIn, 
		ID3D11DepthStencilView* DSVIn,
		Camera* CameraIn, 
		Map* MapIn
	) = 0;
	virtual void ResetRendering() = 0;

public:
	virtual void Render(ID3D11DeviceContext* DeviceContextIn, MeshObject* MeshObjectIn) = 0;
	virtual void Render(ID3D11DeviceContext* DeviceContextIn, ABoundingComponent* MeshObjectIn) = 0;
};
