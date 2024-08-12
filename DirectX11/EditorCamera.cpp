#include "EditorCamera.h"
#include "DefineUtility.h"
#include "GraphicsPipeline.h"

EditorCamera::EditorCamera(GraphicsPipeline* GraphicsPipelineInstance, const UINT& WidthIn, const UINT& HeightIn)
	: Camera(GraphicsPipelineInstance, WidthIn, HeightIn)
{
	ID3D11Device* Device = GraphicsPipelineInstance->GetDevice();

	D3D11_TEXTURE2D_DESC IdTexture2DDesc;
	AutoZeroMemory(IdTexture2DDesc);
	SceneTexture2D->GetDesc(&IdTexture2DDesc);

	Device->CreateTexture2D(&IdTexture2DDesc, NULL, IdSelectTexture2D.GetAddressOf());
	Device->CreateShaderResourceView(IdSelectTexture2D.Get(), NULL, IdSelectSRV.GetAddressOf());
	Device->CreateRenderTargetView(IdSelectTexture2D.Get(), NULL, IdSelectRTV.GetAddressOf());

	D3D11_TEXTURE2D_DESC DepthStencilTexture2DDesc;
	AutoZeroMemory(DepthStencilTexture2DDesc);
	DepthStencilTexture2D->GetDesc(&DepthStencilTexture2DDesc);

	Device->CreateTexture2D(&DepthStencilTexture2DDesc, NULL, IdSelectDepthStencilTexture2D.GetAddressOf());
	Device->CreateDepthStencilView(IdSelectDepthStencilTexture2D.Get(), NULL, IdSelectDSV.GetAddressOf());
}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::CleanupLens()
{
	Camera::CleanupLens();
	DeviceContextCached->ClearRenderTargetView(IdSelectRTV.Get(), ClearColor);
	DeviceContextCached->ClearDepthStencilView(IdSelectDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
