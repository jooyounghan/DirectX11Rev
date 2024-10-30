#pragma once
#include <list>
#include <vector>

#define PSOArgsIn																				\
ID3D11DeviceContext* DeviceContextIn,															\
const Microsoft::WRL::ComPtr<ID3D11InputLayout>& InputLayoutIn,									\
const Microsoft::WRL::ComPtr<ID3D11VertexShader>& VertexShaderIn,								\
const UINT& NumVSConstBuffersIn,																\
const UINT& NumVSSRVsIn,																		\
const Microsoft::WRL::ComPtr<ID3D11HullShader>& HullShaderIn,									\
const UINT& NumHSConstBuffersIn,																\
const UINT& NumHSSRVsIn,																		\
const Microsoft::WRL::ComPtr<ID3D11DomainShader>& DomainShaderIn,								\
const UINT& NumDSConstBuffersIn,																\
const UINT& NumDSSRVsIn,																		\
const Microsoft::WRL::ComPtr<ID3D11PixelShader>& PixelShaderIn,									\
const UINT& NumPSConstBuffersIn,																\
const UINT& NumPSSRVsIn,																		\
const D3D11_PRIMITIVE_TOPOLOGY& PrimitiveTopologyIn,											\
const UINT& NumRenderTargetsIn,																	\
const DXGI_FORMAT* const										RTVFormatsIn,					\
const DXGI_FORMAT& DSVFormatIn,																	\
const DXGI_SAMPLE_DESC& SampleDescIn,															\
const Microsoft::WRL::ComPtr<ID3D11RasterizerState>				RasterizerStateIn,				\
const Microsoft::WRL::ComPtr<ID3D11DepthStencilState>			DepthStencilStateIn,			\
const UINT& StencilRefIn,																		\
const Microsoft::WRL::ComPtr<ID3D11BlendState>					BlendStateIn,					\
const std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>>& SamplerStatesIn					

#define PSOArgs																						\
DeviceContextIn, InputLayoutIn, VertexShaderIn, NumVSConstBuffersIn, NumVSSRVsIn,					\
HullShaderIn, NumHSConstBuffersIn, NumHSSRVsIn, DomainShaderIn, NumDSConstBuffersIn, NumDSSRVsIn,	\
PixelShaderIn, NumPSConstBuffersIn, NumPSSRVsIn, PrimitiveTopologyIn, NumRenderTargetsIn,			\
RTVFormatsIn, DSVFormatIn, SampleDescIn, RasterizerStateIn, DepthStencilStateIn,					\
StencilRefIn, BlendStateIn, SamplerStatesIn

#define CONSTANT_BUFFER_SETTER(Shader)																											\
public:																																			\
		inline void Set##Shader##ConstantBuffers(const size_t& StartSlot, const size_t& NumBuffers, ID3D11Buffer* const* ConstantBuffers)		\
		{																																		\
			memset(&Is##Shader##ConstBufferSet[StartSlot], true, NumBuffers);																	\
			DeviceContextCached->##Shader##SetConstantBuffers(static_cast<UINT>(StartSlot), static_cast<UINT>(NumBuffers), ConstantBuffers);	\
		}

#define SRV_SETTER(Shader)																														\
public:																																			\
		inline void Set##Shader##ShaderResourceViews(const size_t& StartSlot, const size_t& NumViews, ID3D11ShaderResourceView* const* SRVs)	\
		{																																		\
			memset(&Is##Shader##SRVSet[StartSlot], true, NumViews);																				\
			DeviceContextCached->##Shader##SetShaderResources(static_cast<UINT>(StartSlot), static_cast<UINT>(NumViews), SRVs);					\
		}

#define CONSTANT_BUFFER_RESETTER(Shader)																		\
public:																											\
		inline void Reset##Shader##ConstantBuffers(const size_t& StartSlot, const size_t& NumBuffers)			\
		{																										\
			memset(&Is##Shader##ConstBufferSet[StartSlot], false, NumBuffers);									\
			std::vector<ID3D11Buffer*> ConstantBuffers;															\
			ConstantBuffers.resize(NumBuffers, nullptr);														\
			DeviceContextCached->##Shader##SetConstantBuffers(static_cast<UINT>(StartSlot), static_cast<UINT>(NumBuffers), ConstantBuffers.data());	\
		}

#define SRV_RESETTER(Shader)																					\
public:																											\
		inline void Reset##Shader##ShaderResourceViews(const size_t& StartSlot, const size_t& NumViews)			\
		{																										\
			memset(&Is##Shader##SRVSet[StartSlot], false, NumViews);											\
			std::vector<ID3D11ShaderResourceView*> SRVs;														\
			SRVs.resize(NumViews, nullptr);																		\
			DeviceContextCached->##Shader##SetShaderResources(static_cast<UINT>(StartSlot), static_cast<UINT>(NumViews), SRVs.data());				\
		}

#define RESET_RESOURCE_FLAG(Shader)								\
memset(Is##Shader##ConstBufferSet, false, CONST_BUFFER_MAX_COUNT);		\
memset(Is##Shader##SRVSet, false, SRV_MAX_COUNT);	

#define CHECK_VALIDATION(Shader)																						    \
for (UINT Shader##CBIdx = 0; Shader##CBIdx < Num##Shader##ConstBuffers; ++Shader##CBIdx)						            \
{																															\
	assert(Is##Shader##ConstBufferSet[Shader##CBIdx] == true);													            \
}																												            \
for (UINT Shader##CBIdx = Num##Shader##ConstBuffers; Shader##CBIdx < CONST_BUFFER_MAX_COUNT; ++Shader##CBIdx)				\
{																												            \
	assert(Is##Shader##ConstBufferSet[Shader##CBIdx] == false);																\
}																															\
for (UINT Shader##SRVsIdx = 0; Shader##SRVsIdx < Num##Shader##SRVs; ++Shader##SRVsIdx)										\
{																															\
	assert(Is##Shader##SRVSet[Shader##SRVsIdx] == true);																		\
}																												            \
for (UINT Shader##SRVsIdx = Num##Shader##SRVs; Shader##SRVsIdx < SRV_MAX_COUNT; ++Shader##SRVsIdx)							\
{																												            \
	assert(Is##Shader##SRVSet[Shader##SRVsIdx] == false);																		\
}