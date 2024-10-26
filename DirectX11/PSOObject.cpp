#include "PSOObject.h"
#include "DefineUtility.h"

using namespace std;
using namespace Microsoft::WRL;

PSOObject::PSOObject(PSOArgsIn)
	: DeviceContextCached(DeviceContextIn), VertexShader(VertexShaderIn),
	InputLayout(InputLayoutIn),  NumVSConstBuffers(NumVSConstBuffersIn), 
	NumVSSRVs(NumVSSRVsIn), PixelShader(PixelShaderIn), 
	HullShader(HullShaderIn),
	NumHSConstBuffers(NumHSConstBuffersIn), NumHSSRVs(NumHSSRVsIn),
	DomainShader(DomainShaderIn),
	NumDSConstBuffers(NumDSConstBuffersIn), NumDSSRVs(NumPSSRVsIn),
	NumPSConstBuffers(NumPSConstBuffersIn), NumPSSRVs(NumPSSRVsIn),
	PrimitiveTopology(PrimitiveTopologyIn), NumRenderTargets(NumRenderTargetsIn), 
	DSVFormat(DSVFormatIn), SampleDesc(SampleDescIn), RasterizerState(RasterizerStateIn),
	DepthStencilState(DepthStencilStateIn), StencilRef(StencilRefIn), BlendState(BlendStateIn),
	SamplerStates(SamplerStatesIn)
{
	memcpy(RTVFormats, RTVFormatsIn, sizeof(DXGI_FORMAT) * NumRenderTargets);

	SamplerStatesCached.clear();
	for (auto& SamplerState : SamplerStates)
	{
		SamplerStatesCached.push_back(SamplerState.Get());
	}
}

PSOObject::~PSOObject()
{
}

void PSOObject::ResetResourceFlag()
{
	RESET_RESOURCE_FLAG(VS);
	RESET_RESOURCE_FLAG(HS);
	RESET_RESOURCE_FLAG(DS);
	RESET_RESOURCE_FLAG(PS);
}

void PSOObject::SetPipelineStateObject(
	const size_t& RTVCountIn, 
	ID3D11RenderTargetView* const* RTVsIn, 
	const D3D11_VIEWPORT* ViewportIn, 
	ID3D11DepthStencilView* DSVIn
)
{
	assert(RTVCountIn == NumRenderTargets);

	ResetResourceFlag();
	D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc;
	for (size_t RtvIndex = 0; RtvIndex < NumRenderTargets; ++RtvIndex)
	{
		const DXGI_FORMAT& SeletectedRTVFormat = RTVFormats[RtvIndex];
		RTVsIn[RtvIndex]->GetDesc(&RenderTargetViewDesc);
		assert(RenderTargetViewDesc.Format == SeletectedRTVFormat);

		if (SampleDesc.Count > 1)
		{
			assert(RenderTargetViewDesc.ViewDimension == D3D11_RTV_DIMENSION_TEXTURE2DMS);
		}
		else
		{
			assert(RenderTargetViewDesc.ViewDimension == D3D11_RTV_DIMENSION_TEXTURE2D);
		}
	}


	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	DSVIn->GetDesc(&DepthStencilViewDesc);
	assert(DepthStencilViewDesc.Format == DSVFormat);

	DeviceContextCached->IASetInputLayout(InputLayout.Get());
	DeviceContextCached->IASetPrimitiveTopology(PrimitiveTopology);

	DeviceContextCached->VSSetShader(VertexShader.Get(), NULL, NULL);
	DeviceContextCached->HSSetShader(HullShader.Get(), NULL, NULL);
	DeviceContextCached->DSSetShader(DomainShader.Get(), NULL, NULL);
	DeviceContextCached->PSSetShader(PixelShader.Get(), NULL, NULL);

	DeviceContextCached->VSSetSamplers(0, static_cast<UINT>(SamplerStates.size()), SamplerStatesCached.data());
	DeviceContextCached->HSSetSamplers(0, static_cast<UINT>(SamplerStates.size()), SamplerStatesCached.data());
	DeviceContextCached->DSSetSamplers(0, static_cast<UINT>(SamplerStates.size()), SamplerStatesCached.data());
	DeviceContextCached->PSSetSamplers(0, static_cast<UINT>(SamplerStates.size()), SamplerStatesCached.data());

	DeviceContextCached->RSSetState(RasterizerState.Get());
	DeviceContextCached->RSSetViewports(1, ViewportIn);

	DeviceContextCached->OMSetDepthStencilState(DepthStencilState.Get(), StencilRef);

	const FLOAT BlendFactor[4] = { 1.f, 1.f, 1.f, 1.f };
	DeviceContextCached->OMSetBlendState(BlendState.Get(), BlendFactor, 0xFFFFFFFF);
	DeviceContextCached->OMSetRenderTargets(static_cast<UINT>(NumRenderTargets), RTVsIn, DSVIn);
}


void PSOObject::CheckPipelineValidation()
{
	CHECK_VALIDATION(VS);
	CHECK_VALIDATION(HS);
	CHECK_VALIDATION(DS);
	CHECK_VALIDATION(PS);
}
