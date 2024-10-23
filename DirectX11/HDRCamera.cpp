#include "HDRCamera.h"

using namespace std;

string HDRCamera::HDRCameraKind = "HDR Camera";

HDRCamera::HDRCamera(MapAsset* MapAssetInstance, const UINT& WidthIn, const UINT& HeightIn)
	: ACamera(MapAssetInstance, WidthIn, HeightIn, HDRCamera::HDRCameraKind)
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
	DeviceContextCached->ClearRenderTargetView(SDRSceneRTV.Get(), ClearColor);
	DeviceContextCached->ClearDepthStencilView(SceneDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	DeviceContextCached->ClearRenderTargetView(IdSelectRTV.Get(), IDClearColor);
	DeviceContextCached->ClearDepthStencilView(IdSelectDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	DeviceContextCached->ClearRenderTargetView(HDRSceneRTV.Get(), ClearColor);
}
