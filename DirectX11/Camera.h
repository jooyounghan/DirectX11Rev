#pragma once

#include "Viewable.h"
#include "HeaderHelper.h"
#include "IDColor.h"

#include <memory>

class GraphicsPipeline;
class BoundingFrustumObject;

class Camera : public Viewable
{
public: 
	Camera(MapAsset* MapAssetInstance, const UINT& WidthIn, const UINT& HeightIn);
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
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				IdSelectTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	IdSelectSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		IdSelectRTV;
	MakeComPtrGetter(IdSelectTexture2D);
	MakeComPtrGetter(IdSelectSRV);
	MakeComPtrGetter(IdSelectRTV);

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				IdStagingTexture2D;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				IdSelectDepthStencilTexture2D;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		IdSelectDSV;
	MakeComPtrGetter(IdSelectDSV);

protected:
	static const FLOAT ClearColor[4];
	static const FLOAT IDClearColor[4];

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	UINT GetID(
		const float& RelativeMousePosX, const float& RelativeMousePosY,
		const float& WindowSizeX, const float& WindowSizeY
	);

public:
	virtual void CleanupLens();

public:
	virtual void Render() override final;
};

