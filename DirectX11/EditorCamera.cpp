#include "EditorCamera.h"
#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "DefineUtility.h"

const FLOAT EditorCamera::IDClearColor[4] = { 1.f, 1.f, 1.f, 1.f };

EditorCamera::EditorCamera(const UINT& WidthIn, const UINT& HeightIn)
	: Camera(WidthIn, HeightIn)
{
	ID3D11Device* Device = App::GGraphicPipeline->GetDevice();

	D3D11_TEXTURE2D_DESC IdTexture2DDesc;
	AutoZeroMemory(IdTexture2DDesc);
	SceneTexture2D->GetDesc(&IdTexture2DDesc);

	IdTexture2DDesc.SampleDesc.Count = 1;
	IdTexture2DDesc.SampleDesc.Quality = 0;

	Device->CreateTexture2D(&IdTexture2DDesc, NULL, IdSelectTexture2D.GetAddressOf());
	Device->CreateShaderResourceView(IdSelectTexture2D.Get(), NULL, IdSelectSRV.GetAddressOf());
	Device->CreateRenderTargetView(IdSelectTexture2D.Get(), NULL, IdSelectRTV.GetAddressOf());

	IdTexture2DDesc.Width = 1;
	IdTexture2DDesc.Height = 1;
	IdTexture2DDesc.Usage = D3D11_USAGE_STAGING;
	IdTexture2DDesc.BindFlags = NULL;
	IdTexture2DDesc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;

	Device->CreateTexture2D(&IdTexture2DDesc, NULL, IdStagingTexture2D.GetAddressOf());

	D3D11_TEXTURE2D_DESC DepthStencilTexture2DDesc;
	AutoZeroMemory(DepthStencilTexture2DDesc);
	DepthStencilTexture2D->GetDesc(&DepthStencilTexture2DDesc);

	DepthStencilTexture2DDesc.SampleDesc.Count = 1;
	DepthStencilTexture2DDesc.SampleDesc.Quality = 0;

	Device->CreateTexture2D(&DepthStencilTexture2DDesc, NULL, IdSelectDepthStencilTexture2D.GetAddressOf());
	Device->CreateDepthStencilView(IdSelectDepthStencilTexture2D.Get(), NULL, IdSelectDSV.GetAddressOf());
}

EditorCamera::~EditorCamera()
{
}

UINT EditorCamera::GetID(const float& RelativeMousePosX, const float& RelativeMousePosY, const float& WindowSizeX, const float& WindowSizeY)
{
	UINT TextureXPos = static_cast<UINT>((RelativeMousePosX / WindowSizeX) * Viewport.Width);
	UINT TextureYPos = static_cast<UINT>((RelativeMousePosY / WindowSizeY) * Viewport.Height);

	D3D11_BOX BoxData;
	AutoZeroMemory(BoxData);
	BoxData.left = TextureXPos;
	BoxData.right = TextureXPos + 1;
	BoxData.top = TextureYPos;
	BoxData.bottom = TextureYPos + 1;
	BoxData.front = 0;
	BoxData.back = 1;

	DeviceContextCached->CopySubresourceRegion(IdStagingTexture2D.Get(), 0, 0, 0, 0, IdSelectTexture2D.Get(), 0, &BoxData);

	UINT IDResult;

	D3D11_MAPPED_SUBRESOURCE MappedSubResource;
	AutoZeroMemory(MappedSubResource);
	DeviceContextCached->Map(IdStagingTexture2D.Get(), 0, D3D11_MAP_READ, NULL, &MappedSubResource);
	memcpy(&IDResult, MappedSubResource.pData, sizeof(uint32_t));
	DeviceContextCached->Unmap(IdStagingTexture2D.Get(), 0);

	return IDResult;
}

void EditorCamera::CleanupLens()
{
	Camera::CleanupLens();
	DeviceContextCached->ClearRenderTargetView(IdSelectRTV.Get(), IDClearColor);
	DeviceContextCached->ClearDepthStencilView(IdSelectDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
