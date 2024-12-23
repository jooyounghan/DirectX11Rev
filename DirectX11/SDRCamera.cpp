#include "SDRCamera.h"

using namespace std;

string SDRCamera::SDRCameraKind = "SDR Camera";

SDRCamera::SDRCamera(const UINT& WidthIn, const UINT& HeightIn)
	: ACamera(WidthIn, HeightIn, SDRCamera::SDRCameraKind)
{
}

SDRCamera::~SDRCamera()
{
}

void SDRCamera::CleanupLens()
{
	DeviceContextCached->ClearRenderTargetView(SDRSceneRTV.Get(), ClearColor);
	DeviceContextCached->ClearRenderTargetView(IdSelectRTV.Get(), IDClearColor);

	for (size_t idx = 0; idx < GBufferCount; ++idx)
	{
		DeviceContextCached->ClearRenderTargetView(GBufferRTVs[idx].Get(), ClearColor);
	}

	DeviceContextCached->ClearDepthStencilView(SceneDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	DeviceContextCached->ClearDepthStencilView(GBufferDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
