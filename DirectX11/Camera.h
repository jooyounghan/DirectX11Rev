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
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				SDRSceneTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	SDRSceneSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		SDRSceneRTV;
	MakeComPtrGetter(SDRSceneTexture2D);
	MakeComPtrGetter(SDRSceneSRV);
	MakeComPtrGetter(SDRSceneRTV);

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				DepthStencilTexture2D;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		SceneDSV;
	MakeComPtrGetter(SceneDSV);

protected:
	static const FLOAT ClearColor[4];

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void CleanupLens();
};

