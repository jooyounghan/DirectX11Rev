#pragma once
#include "Camera.h"

class HDRCamera : public Camera
{
public:
	HDRCamera(MapAsset* MapAssetInstance, const UINT& WidthIn, const UINT& HeightIn);
	virtual ~HDRCamera();;

public:
	static const char* HDRCameraIdentifier;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				HDRSceneTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	HDRSceneSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		HDRSceneRTV;
	MakeComPtrGetter(HDRSceneTexture2D);
	MakeComPtrGetter(HDRSceneSRV);
	MakeComPtrGetter(HDRSceneRTV);

public:
	virtual void CleanupLens() override;
};

