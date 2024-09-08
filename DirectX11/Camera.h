#pragma once

#include "Viewable.h"
#include "HeaderHelper.h"
#include <memory>

class GraphicsPipeline;
class BoundingFrustumObject;

class Camera : public Viewable
{
public: 
	Camera(const UINT& WidthIn, const UINT& HeightIn);
	virtual ~Camera();

public:
	static const char* CameraIdentifier;

protected:
	std::unique_ptr<BoundingFrustumObject> CamearaFrustum;
	MakeSmartPtrGetter(CamearaFrustum);

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				SceneTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	SceneSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		SceneRTV;
	MakeComPtrGetter(SceneTexture2D);
	MakeComPtrGetter(SceneSRV);
	MakeComPtrGetter(SceneRTV);

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				DepthStencilTexture2D;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		SceneDSV;
	MakeComPtrGetter(SceneDSV);

protected:
	const FLOAT ClearColor[4] = { 0.2f, 0.2f, 0.2f, 1.f };

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptRenderer(ARenderer* Renderer) override;

public:
	virtual void CleanupLens();
};

