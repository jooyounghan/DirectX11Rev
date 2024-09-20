#pragma once
#include "APSOObject.h"
#include <list>

class ABoundingObject;

class BoundingObjectPSO : public APSOObject
{
public:
	BoundingObjectPSO(
		ID3D11DeviceContext* DeviceContextIn,
		const Microsoft::WRL::ComPtr<ID3D11InputLayout>& InputLayoutIn,
		const Microsoft::WRL::ComPtr<ID3D11VertexShader>& VertexShaderIn,
		const UINT& NumVSConstBuffersIn,
		const UINT& NumVSSRVsIn,
		const Microsoft::WRL::ComPtr<ID3D11PixelShader>& PixelShaderIn,
		const UINT& NumPSConstBuffersIn,
		const UINT& NumPSSRVsIn,
		const D3D11_PRIMITIVE_TOPOLOGY& PrimitiveTopologyIn,
		const UINT& NumRenderTargetsIn,
		const DXGI_FORMAT* const										RTVFormatsIn,
		const DXGI_FORMAT& DSVFormatIn,
		const DXGI_SAMPLE_DESC& SampleDescIn,
		const Microsoft::WRL::ComPtr<ID3D11RasterizerState>				RasterizerStateIn,
		const Microsoft::WRL::ComPtr<ID3D11DepthStencilState>			DepthStencilStateIn,
		const UINT& StencilRefIn,
		const Microsoft::WRL::ComPtr<ID3D11BlendState>					BlendStateIn,
		const std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>>& SamplerStatesIn
	);
	virtual ~BoundingObjectPSO();

protected:
	ManageTargetObjects(ABoundingObject);

public:
	virtual void PresetRendering(Camera* CameraIn, MapAsset* MapIn) override;
	virtual void Render() override;
	virtual void ResetRendering() override;
};