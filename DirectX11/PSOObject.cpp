#include "PSOObject.h"
#include "DefineUtility.h"

PSOObject::PSOObject(
	ID3D11DeviceContext*										DeviceContextIn,
	const Microsoft::WRL::ComPtr<ID3D11InputLayout>&			InputLayoutIn, 
	const Microsoft::WRL::ComPtr<ID3D11VertexShader>&			VertexShaderIn, 
	const UINT&													NumVSConstBuffersIn,
	const UINT&													NumVSSRVsIn,
	const Microsoft::WRL::ComPtr<ID3D11PixelShader>&			PixelShaderIn, 
	const UINT&													NumPSConstBuffersIn,
	const UINT&													NumPSSRVsIn,
	const D3D11_PRIMITIVE_TOPOLOGY&								PrimitiveTopologyIn, 
	const UINT&													NumRenderTargetsIn, 
	const DXGI_FORMAT*											RTVFormatsIn, 
	const DXGI_FORMAT &											DSVFormatIn, 
	const DXGI_SAMPLE_DESC&										SampleDeskIn, 
	const Microsoft::WRL::ComPtr<ID3D11RasterizerState>			RasterizerStateIn, 
	const Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		DepthStencilStateIn, 
	const UINT&													StencilRefIn,
	const Microsoft::WRL::ComPtr<ID3D11BlendState>				BlendStateIn
)
	: DeviceContextCached(DeviceContextIn), VertexShader(VertexShaderIn),
	InputLayout(InputLayoutIn),  NumVSConstBuffers(NumVSConstBuffersIn), 
	NumVSSRVs(NumVSSRVsIn), PixelShader(PixelShaderIn), 
	NumPSConstBuffers(NumPSConstBuffersIn), NumPSSRVs(NumPSSRVsIn),
	PrimitiveTopology(PrimitiveTopologyIn), NumRenderTargets(NumRenderTargetsIn), 
	DSVFormat(DSVFormatIn), SampleDesk(SampleDeskIn), RasterizerState(RasterizerStateIn),
	DepthStencilState(DepthStencilStateIn), StencilRef(StencilRefIn), BlendState(BlendStateIn)
{
	memcpy(RTVFormats, RTVFormatsIn, sizeof(DXGI_FORMAT) * NumRenderTargets);
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


void PSOObject::SetPipelineObject(UINT RTVCountIn, ID3D11RenderTargetView** RTVsIn, D3D11_VIEWPORT* ViewportIn, ID3D11DepthStencilView* DSVIn)
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
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	DSVIn->GetDesc(&DepthStencilViewDesc);
	assert(DepthStencilViewDesc.Format == DSVFormat);
#endif // _DEBUG

	DeviceContextCached->IASetInputLayout(InputLayout.Get());
	DeviceContextCached->IASetPrimitiveTopology(PrimitiveTopology);

	DeviceContextCached->VSSetShader(VertexShader.Get(), NULL, NULL);
	DeviceContextCached->PSSetShader(PixelShader.Get(), NULL, NULL);

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

	for (UINT VsSRVIdx = 0; VsSRVIdx < NumVSSRVs; ++VsSRVIdx)
	{
		assert(IsVSSRVSet[VsSRVIdx] == true);
	}

	for (UINT PsCBIdx = 0; PsCBIdx < NumPSConstBuffers; ++PsCBIdx)
	{
		assert(IsPSConstBufferSet[PsCBIdx] == true);
	}

	for (UINT PsSRVIdx = 0; PsSRVIdx < NumPSSRVs; ++PsSRVIdx)
	{
		assert(IsPSSRVSet[PsSRVIdx] == true);
	}
}
