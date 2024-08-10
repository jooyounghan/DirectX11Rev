#include "EditorCamera.h"
#include "DefineUtility.h"
#include "GraphicsPipeline.h"

EditorCamera::EditorCamera(GraphicsPipeline* GraphicsPipelineInstance, const UINT& WidthIn, const UINT& HeightIn)
	: Camera(GraphicsPipelineInstance, WidthIn, HeightIn)
{
	ID3D11Device* Device = GraphicsPipelineInstance->GetDevice();

	D3D11_TEXTURE2D_DESC IdTexture2DDesc;
	AutoZeroMemory(IdTexture2DDesc);
	IdTexture2DDesc.Width = WidthIn;
	IdTexture2DDesc.Height = HeightIn;
	IdTexture2DDesc.MipLevels = 1;
	IdTexture2DDesc.ArraySize = 1;
	IdTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	IdTexture2DDesc.Format = DXGI_FORMAT_R32_UINT;
	IdTexture2DDesc.SampleDesc.Count = 1;
	IdTexture2DDesc.SampleDesc.Quality = 0;
	IdTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	IdTexture2DDesc.CPUAccessFlags = NULL;
	IdTexture2DDesc.MiscFlags = NULL;

	Device->CreateTexture2D(&IdTexture2DDesc, NULL, IdSelectTexture2D.GetAddressOf());
	Device->CreateShaderResourceView(IdSelectTexture2D.Get(), NULL, IdSelectSRV.GetAddressOf());
	Device->CreateRenderTargetView(IdSelectTexture2D.Get(), NULL, IdSelectRTV.GetAddressOf());

}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::CleanupLens()
{
	Camera::CleanupLens();
	DeviceContextCached->ClearRenderTargetView(IdSelectRTV.Get(), ClearColor);
}
