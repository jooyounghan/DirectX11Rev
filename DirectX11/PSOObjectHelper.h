#pragma once
#include <list>
#include <vector>

#define PSOArgsIn																				\
ID3D11DeviceContext* DeviceContextIn,															\
const Microsoft::WRL::ComPtr<ID3D11InputLayout>& InputLayoutIn,									\
const Microsoft::WRL::ComPtr<ID3D11VertexShader>& VertexShaderIn,								\
const UINT& NumVSConstBuffersIn,																\
const UINT& NumVSSRVsIn,																		\
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

#define PSOArgs																					\
DeviceContextIn, InputLayoutIn, VertexShaderIn, NumVSConstBuffersIn, NumVSSRVsIn,				\
PixelShaderIn, NumPSConstBuffersIn, NumPSSRVsIn, PrimitiveTopologyIn, NumRenderTargetsIn,		\
RTVFormatsIn, DSVFormatIn, SampleDescIn, RasterizerStateIn, DepthStencilStateIn,				\
StencilRefIn, BlendStateIn, SamplerStatesIn

#ifdef _DEBUG
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

	#define CONSTANT_BUFFER_RESETTER(Shader)																		\
	public:																											\
			inline void Reset##Shader##ConstantBuffers(UINT StartSlot, UINT NumBuffers)								\
			{																										\
				memset(&Is##Shader##ConstBufferSet[StartSlot], false, NumBuffers);									\
				std::vector<ID3D11Buffer*> ConstantBuffers;															\
				ConstantBuffers.resize(NumBuffers, nullptr);														\
				DeviceContextCached->##Shader##SetConstantBuffers(StartSlot, NumBuffers, ConstantBuffers.data());	\
			}

	#define SRV_RESETTER(Shader)																					\
	public:																											\
			inline void Reset##Shader##ShaderResourceViews(UINT StartSlot, UINT NumViews)							\
			{																										\
				memset(&Is##Shader##SRVSet[StartSlot], false, NumViews);											\
				std::vector<ID3D11ShaderResourceView*> SRVs;														\
				SRVs.resize(NumViews, nullptr);																		\
				DeviceContextCached->##Shader##SetShaderResources(StartSlot, NumViews, SRVs.data());				\
			}
#else
	#define CONSTANT_BUFFER_SETTER(Shader)																						\
		public:																													\
				inline void Set##Shader##ConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ConstantBuffers)	\
				{																												\
					DeviceContextCached->##Shader##SetConstantBuffers(StartSlot, NumBuffers, ConstantBuffers);					\
				}

	#define SRV_SETTER(Shader)																										\
		public:																														\
				inline void Set##Shader##ShaderResourceViews(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* SRVs)	\
				{																													\
					DeviceContextCached->##Shader##SetShaderResources(StartSlot, NumViews, SRVs);									\
				}

	#define CONSTANT_BUFFER_RESETTER(Shader)																			\
		public:																											\
				inline void Reset##Shader##ConstantBuffers(UINT StartSlot, UINT NumBuffers)								\
				{																										\
					std::vector<ID3D11Buffer*> ConstantBuffers;															\
					ConstantBuffers.resize(NumBuffers, nullptr);														\
					DeviceContextCached->##Shader##SetConstantBuffers(StartSlot, NumBuffers, ConstantBuffers.data());	\
				}

	#define SRV_RESETTER(Shader)																						\
		public:																											\
				inline void Reset##Shader##ShaderResourceViews(UINT StartSlot, UINT NumViews)							\
				{																										\
					std::vector<ID3D11ShaderResourceView*> SRVs;														\
					SRVs.resize(NumViews, nullptr);																		\
					DeviceContextCached->##Shader##SetShaderResources(StartSlot, NumViews, SRVs.data());				\
				}
#endif // DEBUG

