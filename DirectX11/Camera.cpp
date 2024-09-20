#include "Camera.h"
#include "DefineUtility.h"
#include "GraphicsPipeline.h"
#include "BoundingFrustumObject.h"

using namespace std;

const char* Camera::CameraIdentifier = "Camera";
const FLOAT Camera::ClearColor[4] = { 0.2f, 0.2f, 0.2f, 1.f };

Camera::Camera(const UINT& WidthIn, const UINT& HeightIn)
	: Viewable(WidthIn, HeightIn)
{
	CamearaFrustum = make_unique<BoundingFrustumObject>(this);
	CamearaFrustum->SetParent(this, PickingIDBufferCached);

	static size_t CameraCount = 0;
	CameraCount++;

	ObjectName = CameraIdentifier + to_string(CameraCount);
	AttachableKind = EAttachableObjectKind::SDR_CAMERA_KIND;

	ID3D11Device* Device = App::GGraphicPipeline->GetDevice();

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

	Device->CreateTexture2D(&SceneTexture2DDesc, NULL, SDRSceneTexture2D.GetAddressOf());
	Device->CreateShaderResourceView(SDRSceneTexture2D.Get(), NULL, SDRSceneSRV.GetAddressOf());
	Device->CreateRenderTargetView(SDRSceneTexture2D.Get(), NULL, SDRSceneRTV.GetAddressOf());

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

	Device->CreateTexture2D(&DepthStencilTexture2DDesc, NULL, DepthStencilTexture2D.GetAddressOf());
	Device->CreateDepthStencilView(DepthStencilTexture2D.Get(), NULL, SceneDSV.GetAddressOf());
}

Camera::~Camera()
{
}

void Camera::Update(const float& DeltaTimeIn)
{
	Viewable::Update(DeltaTimeIn);
	CamearaFrustum->Update(DeltaTimeIn);
}

void Camera::CleanupLens()
{
	DeviceContextCached->ClearRenderTargetView(SDRSceneRTV.Get(), ClearColor);
	DeviceContextCached->ClearDepthStencilView(SceneDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}