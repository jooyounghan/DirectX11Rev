#pragma once

#include "Viewable.h"
#include "HeaderHelper.h"
#include "IDColor.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <memory>

class GraphicsPipeline;
class BoundingFrustumObject;

enum EGBuffer : size_t
{
	Position_GBuffer,
	BaseColor_GBuffer,
	Normal_GBuffer,
	AO_Metallic_Roughness_GBuffer,
	Emissive_GBuffer,
	GBufferCount
};

class ACamera : public Viewable
{
public: 
	ACamera(
		const UINT& WidthIn, const UINT& HeightIn,
		const std::string& AttachableKindIn
	);
	virtual ~ACamera();

protected:
	std::unique_ptr<BoundingFrustumObject> CamearaFrustum;
	MakeSmartPtrGetter(CamearaFrustum);

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				SDRSceneTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	SDRSceneSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		SDRSceneRTV;
	MakeComPtrGetter(SDRSceneTexture2D);
	MakeComPtrGetter(SDRSceneSRV);
	MakeComPtrGetter(SDRSceneRTV);

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				SceneDSTexture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		SceneDSV;
	MakeComPtrGetter(SceneDSV);

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				IdSelectTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	IdSelectSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		IdSelectRTV;
	MakeComPtrGetter(IdSelectTexture2D);
	MakeComPtrGetter(IdSelectSRV);
	MakeComPtrGetter(IdSelectRTV);

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				IdStagingTexture2D;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				GBufferTexture2Ds[GBufferCount];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	GBufferSRVs[GBufferCount];
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		GBufferRTVs[GBufferCount];


protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				GBufferDSTexture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		GBufferDSV;
	MakeComPtrGetter(GBufferDSV);

public:
	ID3D11ShaderResourceView* GetGBufferSRV(EGBuffer GBufferIdx);
	ID3D11RenderTargetView* GetGBufferRTV(EGBuffer GBufferIdx);


protected:
	static const FLOAT ClearColor[4];
	static const FLOAT IDClearColor[4];

public:
	virtual void CleanupLens() = 0;
	virtual void Update(const float& DeltaTimeIn) override;
	virtual void Render(MapAsset* MapAssetIn) override final;
	UINT GetID(
		const float& RelativeMousePosX, const float& RelativeMousePosY,
		const float& WindowSizeX, const float& WindowSizeY
	);

protected:
	VertexBuffer<DirectX::XMFLOAT3> ResolveSquarePositionBuffer;
	VertexBuffer<DirectX::XMFLOAT2> ResolveSquareUVBuffer;
	IndexBuffer<uint8_t>	ResolveSquareIndexBuffer;
	MakeGetter(ResolveSquarePositionBuffer);
	MakeGetter(ResolveSquareUVBuffer);
	MakeGetter(ResolveSquareIndexBuffer);
};

