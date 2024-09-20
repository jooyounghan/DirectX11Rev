#pragma once
#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include "PSOObjectHelper.h"

#define CONST_BUFFER_MAX_COUNT	16
#define SRV_MAX_COUNT			128


class PSOObject
{
public:
	PSOObject(PSOArgsIn);
	~PSOObject();

protected:
	ID3D11DeviceContext*							DeviceContextCached = nullptr;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		VertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		InputLayout;
	UINT											NumVSConstBuffers;
	UINT											NumVSSRVs;

	Microsoft::WRL::ComPtr<ID3D11PixelShader>		PixelShader;
	UINT											NumPSConstBuffers;
	UINT											NumPSSRVs;

#ifdef _DEBUG
private:
	bool											IsVSConstBufferSet[CONST_BUFFER_MAX_COUNT];
	bool											IsVSSRVSet[SRV_MAX_COUNT];
	bool											IsPSConstBufferSet[CONST_BUFFER_MAX_COUNT];
	bool											IsPSSRVSet[SRV_MAX_COUNT];

private:
	void ResetResourceFlag();

#endif // _DEBUG


	D3D11_PRIMITIVE_TOPOLOGY						PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	UINT											NumRenderTargets = 0;
	DXGI_FORMAT										RTVFormats[8]{ DXGI_FORMAT_UNKNOWN };
	DXGI_FORMAT										DSVFormat = DXGI_FORMAT_UNKNOWN;
	DXGI_SAMPLE_DESC								SampleDesc;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	RasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState;
	UINT											StencilRef;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		BlendState;	

private:
	std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> SamplerStates;
	std::vector<ID3D11SamplerState*> SamplerStatesCached;

public:
	void SetPipelineStateObject(
		const UINT&						RTVCountIn,
		ID3D11RenderTargetView**		RTVsIn,
		const D3D11_VIEWPORT*			ViewportIn,
		ID3D11DepthStencilView*	DSVIn
	);

public:
	CONSTANT_BUFFER_SETTER(VS);
	CONSTANT_BUFFER_RESETTER(VS);
	SRV_SETTER(VS);
	SRV_RESETTER(VS);

public:
	CONSTANT_BUFFER_SETTER(PS);
	CONSTANT_BUFFER_RESETTER(PS);
	SRV_SETTER(PS);
	SRV_RESETTER(PS);

public:
	void CheckPipelineValidation();
};

