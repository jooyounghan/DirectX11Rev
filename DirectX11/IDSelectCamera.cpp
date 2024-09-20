#include "IDSelectCamera.h"
#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "DefineUtility.h"

using namespace std;

const char* IDSelectCamera::IDSelectCameraIdentifier = "ID Select Camera";
const FLOAT IDSelectCamera::IDClearColor[4] = { 1.f, 1.f, 1.f, 1.f };

IDSelectCamera::IDSelectCamera(const UINT& WidthIn, const UINT& HeightIn)
	: Viewable(WidthIn, HeightIn)
{
	static size_t IDSelectCameraCount = 0;
	IDSelectCameraCount++;

	ObjectName = IDSelectCameraIdentifier + to_string(IDSelectCameraCount);
	AttachableKind = EAttachableObjectKind::ID_SELECT_CAMERA_KIND;

	ID3D11Device* Device = App::GGraphicPipeline->GetDevice();

	D3D11_TEXTURE2D_DESC IdTexture2DDesc;
	AutoZeroMemory(IdTexture2DDesc);

	D3D11_TEXTURE2D_DESC SceneTexture2DDesc;
	AutoZeroMemory(SceneTexture2DDesc);
	SceneTexture2DDesc.Width = WidthIn;
	SceneTexture2DDesc.Height = HeightIn;
	SceneTexture2DDesc.MipLevels = 1;
	SceneTexture2DDesc.ArraySize = 1;
	SceneTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	SceneTexture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	SceneTexture2DDesc.SampleDesc.Count = 1;
	SceneTexture2DDesc.SampleDesc.Quality = 0;

	SceneTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	SceneTexture2DDesc.CPUAccessFlags = NULL;
	SceneTexture2DDesc.MiscFlags = NULL;

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
	DepthStencilTexture2DDesc.Width = WidthIn;
	DepthStencilTexture2DDesc.Height = HeightIn;
	DepthStencilTexture2DDesc.MipLevels = 1;
	DepthStencilTexture2DDesc.ArraySize = 1;
	DepthStencilTexture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencilTexture2DDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	DepthStencilTexture2DDesc.SampleDesc.Count = 1;
	DepthStencilTexture2DDesc.SampleDesc.Quality = 0;
	DepthStencilTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthStencilTexture2DDesc.CPUAccessFlags = NULL;
	DepthStencilTexture2DDesc.MiscFlags = NULL;

	Device->CreateTexture2D(&DepthStencilTexture2DDesc, NULL, IdSelectDepthStencilTexture2D.GetAddressOf());
	Device->CreateDepthStencilView(IdSelectDepthStencilTexture2D.Get(), NULL, IdSelectDSV.GetAddressOf());
}

IDSelectCamera::~IDSelectCamera()
{
}

UINT IDSelectCamera::GetID(const float& RelativeMousePosX, const float& RelativeMousePosY, const float& WindowSizeX, const float& WindowSizeY)
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

void IDSelectCamera::Update(const float& DeltaTimeIn)
{
	Viewable::Update(DeltaTimeIn);
}

void IDSelectCamera::CleanupLens()
{
	DeviceContextCached->ClearRenderTargetView(IdSelectRTV.Get(), IDClearColor);
	DeviceContextCached->ClearDepthStencilView(IdSelectDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
