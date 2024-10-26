#include "PSOObject.h"
#include "DefineUtility.h"

using namespace std;
using namespace Microsoft::WRL;

PSOObject::PSOObject(PSOArgsIn)
	: DeviceContextCached(DeviceContextIn), VertexShader(VertexShaderIn),
	InputLayout(InputLayoutIn),  NumVSConstBuffers(NumVSConstBuffersIn), 
	NumVSSRVs(NumVSSRVsIn), PixelShader(PixelShaderIn), 
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

#ifdef _DEBUG
void PSOObject::ResetResourceFlag()
{
	memset(IsVSConstBufferSet, false, CONST_BUFFER_MAX_COUNT);
	memset(IsVSSRVSet, false, SRV_MAX_COUNT);
	memset(IsPSConstBufferSet, false, CONST_BUFFER_MAX_COUNT);
	memset(IsPSSRVSet, false, SRV_MAX_COUNT);
}
#endif // _DEBUG


void PSOObject::SetPipelineStateObject(
	const UINT& RTVCountIn, 
	ID3D11RenderTargetView* const* RTVsIn, 
	const D3D11_VIEWPORT* ViewportIn, 
	ID3D11DepthStencilView* DSVIn
)
{
	assert(RTVCountIn == NumRenderTargets);

#ifdef _DEBUG
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

#endif // _DEBUG

	DeviceContextCached->IASetInputLayout(InputLayout.Get());
	DeviceContextCached->IASetPrimitiveTopology(PrimitiveTopology);

	DeviceContextCached->VSSetShader(VertexShader.Get(), NULL, NULL);
	DeviceContextCached->PSSetShader(PixelShader.Get(), NULL, NULL);

	DeviceContextCached->VSSetSamplers(0, static_cast<UINT>(SamplerStates.size()), SamplerStatesCached.data());
	DeviceContextCached->PSSetSamplers(0, static_cast<UINT>(SamplerStates.size()), SamplerStatesCached.data());

	DeviceContextCached->RSSetState(RasterizerState.Get());
	DeviceContextCached->RSSetViewports(1, ViewportIn);

	DeviceContextCached->OMSetDepthStencilState(DepthStencilState.Get(), StencilRef);

	const FLOAT BlendFactor[4] = { 1.f, 1.f, 1.f, 1.f };
	DeviceContextCached->OMSetBlendState(BlendState.Get(), BlendFactor, 0xFFFFFFFF);
	DeviceContextCached->OMSetRenderTargets(NumRenderTargets, RTVsIn, DSVIn);
}


void PSOObject::CheckPipelineValidation()
{
	for (UINT VsCBIdx = 0; VsCBIdx < NumVSConstBuffers; ++VsCBIdx)
	{
		assert(IsVSConstBufferSet[VsCBIdx] == true);
	}
	for (UINT VsCBIdx = NumVSConstBuffers; VsCBIdx < CONST_BUFFER_MAX_COUNT; ++VsCBIdx)
	{
		assert(IsVSConstBufferSet[VsCBIdx] == false);
	}

	for (UINT VsSRVIdx = 0; VsSRVIdx < NumVSSRVs; ++VsSRVIdx)
	{
		assert(IsVSSRVSet[VsSRVIdx] == true);
	}
	for (UINT VsSRVIdx = NumVSSRVs; VsSRVIdx < SRV_MAX_COUNT; ++VsSRVIdx)
	{
		assert(IsVSSRVSet[VsSRVIdx] == false);
	}

	for (UINT PsCBIdx = 0; PsCBIdx < NumPSConstBuffers; ++PsCBIdx)
	{
		assert(IsPSConstBufferSet[PsCBIdx] == true);
	}
	for (UINT PsCBIdx = NumPSConstBuffers; PsCBIdx < CONST_BUFFER_MAX_COUNT; ++PsCBIdx)
	{
		assert(IsPSConstBufferSet[PsCBIdx] == false);
	}

	for (UINT PsSRVIdx = 0; PsSRVIdx < NumPSSRVs; ++PsSRVIdx)
	{
		assert(IsPSSRVSet[PsSRVIdx] == true);
	}
	for (UINT PsSRVIdx = NumPSSRVs; PsSRVIdx < SRV_MAX_COUNT; ++PsSRVIdx)
	{
		assert(IsPSSRVSet[PsSRVIdx] == false);
	}
}
