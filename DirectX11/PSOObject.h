#pragma once
#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>

#define CONST_BUFFER_MAX_COUNT	16
#define SRV_MAX_COUNT			128

#define CONSTANT_BUFFER_SETTER(Shader)																					\
public:																													\
		inline void Set##Shader##ConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ConstantBuffers)	\
		{																												\
			memset(&Is##Shader##ConstBufferSet[StartSlot], true, NumBuffers);											\
			DeviceContextCached->##Shader##SetConstantBuffers(StartSlot, NumBuffers, ConstantBuffers);					\
		}

#define SRV_SETTER(Shader)																									\
public:																														\
		inline void Set##Shader##ShaderResourceViews(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* SRVs)	\
		{																													\
			memset(&Is##Shader##SRVSet[StartSlot], true, NumViews);															\
			DeviceContextCached->##Shader##SetShaderResources(StartSlot, NumViews, SRVs);									\
		}


class PSOObject
{
public:
	PSOObject(
		ID3D11DeviceContext*											DeviceContextIn,
		const Microsoft::WRL::ComPtr<ID3D11InputLayout>&				InputLayoutIn,
		const Microsoft::WRL::ComPtr<ID3D11VertexShader>&				VertexShaderIn,
		const UINT&														NumVSConstBuffersIn,
		const UINT&														NumVSSRVsIn,
		const Microsoft::WRL::ComPtr<ID3D11PixelShader>&				PixelShaderIn,
		const UINT&														NumPSConstBuffersIn,
		const UINT&														NumPSSRVsIn,
		const D3D11_PRIMITIVE_TOPOLOGY&									PrimitiveTopologyIn,
		const UINT&														NumRenderTargetsIn,
		const DXGI_FORMAT* const										RTVFormatsIn,
		const DXGI_FORMAT&												DSVFormatIn,
		const DXGI_SAMPLE_DESC&											SampleDeskIn,
		const Microsoft::WRL::ComPtr<ID3D11RasterizerState>				RasterizerStateIn,
		const Microsoft::WRL::ComPtr<ID3D11DepthStencilState>			DepthStencilStateIn,
		const UINT&														StencilRefIn,
		const Microsoft::WRL::ComPtr<ID3D11BlendState>					BlendStateIn
	);
	~PSOObject();

private:
	ID3D11DeviceContext*							DeviceContextCached = nullptr;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		VertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		InputLayout;
	UINT											NumVSConstBuffers;
	bool											IsVSConstBufferSet[CONST_BUFFER_MAX_COUNT];
	UINT											NumVSSRVs;
	bool											IsVSSRVSet[SRV_MAX_COUNT];

	Microsoft::WRL::ComPtr<ID3D11PixelShader>		PixelShader;
	UINT											NumPSConstBuffers;
	bool											IsPSConstBufferSet[CONST_BUFFER_MAX_COUNT];
	UINT											NumPSSRVs;
	bool											IsPSSRVSet[SRV_MAX_COUNT];

	D3D11_PRIMITIVE_TOPOLOGY						PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;;
	UINT											NumRenderTargets = 0;
	DXGI_FORMAT										RTVFormats[8]{ DXGI_FORMAT_UNKNOWN };
	DXGI_FORMAT										DSVFormat = DXGI_FORMAT_UNKNOWN;
	DXGI_SAMPLE_DESC								SampleDesk;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	RasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState;
	UINT											StencilRef;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		BlendState;	

private:
	void ResetResourceFlag();

public:
	void SetPipelineObject(
		UINT						RTVCountIn,
		ID3D11RenderTargetView**	RTVsIn,
		ID3D11DepthStencilView*		DSVIn
	);

	CONSTANT_BUFFER_SETTER(VS);
	SRV_SETTER(VS);
	CONSTANT_BUFFER_SETTER(PS);
	SRV_SETTER(PS);


	void CheckPipelineValidation();
};

