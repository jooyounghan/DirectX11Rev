#pragma once
#include "Camera.h"
#include "IDColor.h"

class EditorCamera : public Camera
{
public:
	EditorCamera(const UINT& WidthIn, const UINT& HeightIn);
	virtual ~EditorCamera();

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

public:
	UINT GetID(
		const float& RelativeMousePosX, const float& RelativeMousePosY,
		const float& WindowSizeX, const float& WindowSizeY
	);

protected:
	static const FLOAT IDClearColor[4];

public:
	virtual void CleanupLens();
};

