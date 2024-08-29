#include "Camera.h"
#include "DefineUtility.h"
#include "GraphicsPipeline.h"
#include "BoundingFrustumObject.h"
#include "ARenderer.h"

using namespace std;

const char* Camera::CameraIdentifier = "Camera";

Camera::Camera(GraphicsPipeline* GraphicsPipelineInstance, const UINT& WidthIn, const UINT& HeightIn)
	: Viewable(GraphicsPipelineInstance, WidthIn, HeightIn)
{
	CamearaFrustum = make_unique<BoundingFrustumObject>(GraphicsPipelineInstance, this);
	CamearaFrustum->SetParentObject(this);

	static size_t CameraCount = 0;
	CameraCount++;

	ObjectName = CameraIdentifier + to_string(CameraCount);
	AttachableKind = EAttachableObjectKind::NormalCameraKind;

	ID3D11Device* Device = GraphicsPipelineInstance->GetDevice();

	D3D11_TEXTURE2D_DESC SceneTexture2DDesc;
	AutoZeroMemory(SceneTexture2DDesc);
	SceneTexture2DDesc.Width = WidthIn;
	SceneTexture2DDesc.Height = HeightIn;
	SceneTexture2DDesc.MipLevels = 1;
	SceneTexture2DDesc.ArraySize = 1;
	SceneTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	SceneTexture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	UINT SampleCount = 0;
	UINT Quality = 0;

	if (FAILED(Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &Quality)))
	{
		SampleCount = 1;
		Quality = 0;
		IsMultiSampling = false;
	}
	else
	{
		SampleCount = 4;
		Quality = Quality - 1;
		IsMultiSampling = true;
	}

	SceneTexture2DDesc.SampleDesc.Count = SampleCount;
	SceneTexture2DDesc.SampleDesc.Quality = Quality;

	SceneTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	SceneTexture2DDesc.CPUAccessFlags = NULL;
	SceneTexture2DDesc.MiscFlags = NULL;

	Device->CreateTexture2D(&SceneTexture2DDesc, NULL, SceneTexture2D.GetAddressOf());
	Device->CreateShaderResourceView(SceneTexture2D.Get(), NULL, SceneSRV.GetAddressOf());
	Device->CreateRenderTargetView(SceneTexture2D.Get(), NULL, SceneRTV.GetAddressOf());

	if (IsMultiSampling)
	{
		SceneTexture2DDesc.SampleDesc.Count = 1;
		SceneTexture2DDesc.SampleDesc.Quality = 0;

		Device->CreateTexture2D(&SceneTexture2DDesc, NULL, ResolvedSceneTexture2D.GetAddressOf());
		Device->CreateShaderResourceView(ResolvedSceneTexture2D.Get(), NULL, ResolvedSceneSRV.GetAddressOf());
		Device->CreateRenderTargetView(ResolvedSceneTexture2D.Get(), NULL, ResolvedSceneRTV.GetAddressOf());
	}
	else
	{
		ResolvedSceneTexture2D = SceneTexture2D;
		ResolvedSceneSRV = SceneSRV;
		ResolvedSceneRTV = SceneRTV;
	}

	D3D11_TEXTURE2D_DESC DepthStencilTexture2DDesc;
	AutoZeroMemory(DepthStencilTexture2DDesc);
	DepthStencilTexture2DDesc.Width = WidthIn;
	DepthStencilTexture2DDesc.Height = HeightIn;
	DepthStencilTexture2DDesc.MipLevels = 1;
	DepthStencilTexture2DDesc.ArraySize = 1;
	DepthStencilTexture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencilTexture2DDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	DepthStencilTexture2DDesc.SampleDesc.Count = SampleCount;
	DepthStencilTexture2DDesc.SampleDesc.Quality = Quality;
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

void Camera::AcceptRenderer(ARenderer* Renderer)
{
	AAttachableObject::AcceptRenderer(Renderer);
	Renderer->Render(DeviceContextCached, CamearaFrustum.get());
}

void Camera::CleanupLens()
{
	if (IsMultiSampling)
	{
		// TODO : Fence 등을 활용하여 병렬 처리 되도록 변경
		DeviceContextCached->ResolveSubresource(ResolvedSceneTexture2D.Get(), NULL, SceneTexture2D.Get(), NULL, DXGI_FORMAT_R8G8B8A8_UNORM);
	}

	DeviceContextCached->ClearRenderTargetView(SceneRTV.Get(), ClearColor);
	DeviceContextCached->ClearDepthStencilView(SceneDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
