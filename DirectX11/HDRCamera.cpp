#include "HDRCamera.h"

using namespace std;

const char* HDRCamera::HDRCameraIdentifier = "HDR Camera";

HDRCamera::HDRCamera(MapAsset* MapAssetInstance, const UINT& WidthIn, const UINT& HeightIn)
	: Camera(MapAssetInstance, WidthIn, HeightIn)
{
	ID3D11Device* Device = App::GGraphicPipeline->GetDevice();

	D3D11_TEXTURE2D_DESC SceneTexture2DDesc;
	AutoZeroMemory(SceneTexture2DDesc);
	SceneTexture2DDesc.Width = WidthIn;
	SceneTexture2DDesc.Height = HeightIn;
	SceneTexture2DDesc.MipLevels = 1;
	SceneTexture2DDesc.ArraySize = 1;
	SceneTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	SceneTexture2DDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

	SceneTexture2DDesc.SampleDesc.Count = 1;
	SceneTexture2DDesc.SampleDesc.Quality = 0;

	SceneTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	SceneTexture2DDesc.CPUAccessFlags = NULL;
	SceneTexture2DDesc.MiscFlags = NULL;

	Device->CreateTexture2D(&SceneTexture2DDesc, NULL, HDRSceneTexture2D.GetAddressOf());
	Device->CreateShaderResourceView(HDRSceneTexture2D.Get(), NULL, HDRSceneSRV.GetAddressOf());
	Device->CreateRenderTargetView(HDRSceneTexture2D.Get(), NULL, HDRSceneRTV.GetAddressOf());
}

HDRCamera::~HDRCamera() {}

void HDRCamera::CleanupLens()
{
	Camera::CleanupLens();
	DeviceContextCached->ClearRenderTargetView(HDRSceneRTV.Get(), ClearColor);
}
