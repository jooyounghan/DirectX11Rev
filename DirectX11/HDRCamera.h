#pragma once
#include "ACamera.h"

class HDRCamera : public ACamera
{
public:
	HDRCamera(const UINT& WidthIn, const UINT& HeightIn);
	virtual ~HDRCamera();

public:
	static std::string HDRCameraKind;

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

