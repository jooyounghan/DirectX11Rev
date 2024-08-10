#pragma once
#include "Camera.h"

class EditorCamera : public Camera
{
public:
	EditorCamera(GraphicsPipeline* GraphicsPipelineInstance, const UINT& WidthIn, const UINT& HeightIn);
	virtual ~EditorCamera();

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				IdSelectTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	IdSelectSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		IdSelectRTV;
	MakeComPtrGetter(IdSelectTexture2D);
	MakeComPtrGetter(IdSelectSRV);
	MakeComPtrGetter(IdSelectRTV);

public:
	virtual void CleanupLens();
};

