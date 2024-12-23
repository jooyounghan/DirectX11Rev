#include "ACamera.h"
#include "DefineUtility.h"
#include "GraphicsPipeline.h"
#include "BoundingFrustumObject.h"

using namespace std;
using namespace DirectX;

const FLOAT ACamera::ClearColor[4] = { 0.2f, 0.2f, 0.2f, 1.f };
const FLOAT ACamera::IDClearColor[4] = { 1.f, 1.f, 1.f, 1.f };

ACamera::ACamera(
	const UINT& WidthIn, const UINT& HeightIn,
	const string& AttachableKindIn
)
	: Viewable(WidthIn, HeightIn, AttachableKindIn)
{
	CamearaFrustum = make_unique<BoundingFrustumObject>(this);
	CamearaFrustum->SetParent(this, PickingIDBufferCached);

	static size_t CameraCount = 0;
	CameraCount++;

	ObjectName = AttachableKindIn + to_string(CameraCount);

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

#pragma region Create SDR Texture
	Device->CreateTexture2D(&SceneTexture2DDesc, NULL, SDRSceneTexture2D.GetAddressOf());
	Device->CreateShaderResourceView(SDRSceneTexture2D.Get(), NULL, SDRSceneSRV.GetAddressOf());
	Device->CreateRenderTargetView(SDRSceneTexture2D.Get(), NULL, SDRSceneRTV.GetAddressOf());
#pragma endregion

#pragma region Create GBuffers
	for (size_t idx = 0; idx < GBufferCount; ++idx)
	{
		Device->CreateTexture2D(&SceneTexture2DDesc, NULL, GBufferTexture2Ds[idx].GetAddressOf());
		Device->CreateShaderResourceView(GBufferTexture2Ds[idx].Get(), NULL, GBufferSRVs[idx].GetAddressOf());
		Device->CreateRenderTargetView(GBufferTexture2Ds[idx].Get(), NULL, GBufferRTVs[idx].GetAddressOf());
	}
#pragma endregion

#pragma region Create ID Texture
	Device->CreateTexture2D(&SceneTexture2DDesc, NULL, IdSelectTexture2D.GetAddressOf());
	Device->CreateShaderResourceView(IdSelectTexture2D.Get(), NULL, IdSelectSRV.GetAddressOf());
	Device->CreateRenderTargetView(IdSelectTexture2D.Get(), NULL, IdSelectRTV.GetAddressOf());
#pragma endregion

#pragma region Create ID Staging Texture
	SceneTexture2DDesc.Width = 1;
	SceneTexture2DDesc.Height = 1;
	SceneTexture2DDesc.Usage = D3D11_USAGE_STAGING;
	SceneTexture2DDesc.BindFlags = NULL;
	SceneTexture2DDesc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;

	Device->CreateTexture2D(&SceneTexture2DDesc, NULL, IdStagingTexture2D.GetAddressOf());
#pragma endregion

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

#pragma region Create SDR Depth Stencil Texture
	Device->CreateTexture2D(&DepthStencilTexture2DDesc, NULL, SceneDSTexture.GetAddressOf());
	Device->CreateDepthStencilView(SceneDSTexture.Get(), NULL, SceneDSV.GetAddressOf());
#pragma endregion

#pragma region Create GBuffer Depth Stencil Texture
	Device->CreateTexture2D(&DepthStencilTexture2DDesc, NULL, GBufferDSTexture.GetAddressOf());
	Device->CreateDepthStencilView(GBufferDSTexture.Get(), NULL, GBufferDSV.GetAddressOf());
#pragma endregion

	vector<XMFLOAT3> ResolverPositions{ 
		{-1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {-1.f, -1.f, 1.f}, 
		{-1.f, -1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, -1.f, 1.f}
	};
	vector<XMFLOAT2> ResolverUVBuffers{ 
		{0.f, 0.f}, {1.f, 0.f}, {0.f, 1.f},
		{0.f, 1.f}, {1.f, 0.f}, {1.f, 1.f},
	};
	vector<uint16_t> ResolverIndex{ 0, 1, 2, 3, 4, 5 };

	//vector<XMFLOAT3> ResolverPositions{ {-1.f, 1.f, 0.f}, {3.f, 1.f, 0.f}, {-1.f, -3.f, 0.f} };
	//vector<XMFLOAT2> ResolverUVBuffers{ {0.f, 0.f}, {2.f, 0.f}, {0.f, 2.f} };
	//vector<uint8_t> ResolverIndex{ 0, 1, 2 };
	ResolveSquarePositionBuffer.InitializeForGPU(ResolverPositions.size(), ResolverPositions.data());
	ResolveSquareUVBuffer.InitializeForGPU(ResolverUVBuffers.size(), ResolverUVBuffers.data());
	ResolveSquareIndexBuffer.InitializeForGPU(ResolverIndex.size(), ResolverIndex.data());
}

ACamera::~ACamera()
{
}

ID3D11ShaderResourceView* ACamera::GetGBufferSRV(EGBuffer GBufferIdx)
{
	return GBufferSRVs[GBufferIdx].Get();
}

ID3D11RenderTargetView* ACamera::GetGBufferRTV(EGBuffer GBufferIdx)
{
	return GBufferRTVs[GBufferIdx].Get();
}

void ACamera::Update(const float& DeltaTimeIn)
{
	Viewable::Update(DeltaTimeIn);
	CamearaFrustum->Update(DeltaTimeIn);
}

UINT ACamera::GetID(const float& RelativeMousePosX, const float& RelativeMousePosY, const float& WindowSizeX, const float& WindowSizeY)
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

void ACamera::Render(MapAsset* MapAssetIn)
{
	Viewable::Render(MapAssetIn);
	CamearaFrustum->Render(MapAssetIn);
}
