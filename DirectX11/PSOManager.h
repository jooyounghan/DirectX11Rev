#pragma once
#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <memory>
#include <unordered_map>

//#define DIRECT_RENDER

enum class EPSOType
{
	// Environment
	Environment_Solid,

	// Position Only
	BoundingObject_Wireframe,

	// Static / Skeletal
	Static_Solid,
	Skeletal_Solid,

	// ID
	BoundingObject_ID_Wireframe,

	// Static / Skeletal
	Static_ID_Solid,
	Skeletal_ID_Solid,

	// Count
	EPSOTypeCount
};

class PSOObject;

struct SPSOArgument
{
	std::vector<ID3D11Buffer*>				VertexBuffers;
	std::vector<UINT>						Strides;
	std::vector<UINT>						Offsets;
	ID3D11Buffer*							IndexBuffer;
	DXGI_FORMAT								IndexFormat;
	UINT									IndexCount;
	std::vector<ID3D11Buffer*>				VSConstantBuffers;
	std::vector<ID3D11ShaderResourceView*>	VSSRVs;
	std::vector<ID3D11Buffer*>				PSConstantBuffers;
	std::vector<ID3D11ShaderResourceView*>	PSSRVs;
	std::vector<ID3D11RenderTargetView*>	RTVs;
	ID3D11DepthStencilView*					DSV;
	const D3D11_VIEWPORT*					Viewport;
};

class PSOManager
{
public:
	PSOManager();
	~PSOManager();

protected:
	std::unordered_map<EPSOType, std::unique_ptr<PSOObject>> PSOObjects;
	
public:
	PSOObject* GetPSOObject(EPSOType PsoTypeIn);

protected:
	std::unordered_map<PSOObject*, std::vector<SPSOArgument>> PSORenderCommandSet;

public:
	inline void AddRenderCommand(PSOObject* PSOObjectIn, const SPSOArgument& PSOArgument) { PSORenderCommandSet[PSOObjectIn].push_back(PSOArgument); }
	void ProcessRender();

private:
	void CreateVertexShader(
		const wchar_t* HlslFileName,
		const D3D11_INPUT_ELEMENT_DESC* InputElementDescs,
		const UINT& NumElements,
		Microsoft::WRL::ComPtr<ID3D11VertexShader>& VertexShader,
		Microsoft::WRL::ComPtr<ID3D11InputLayout>& InputLayout
	);
	void CreatePixelShader(
		const wchar_t* HlslFileName,
		Microsoft::WRL::ComPtr<ID3D11PixelShader>& PixelShader
	);

private:
	void CreateRastersizerState(
		const D3D11_FILL_MODE& FillMode,
		const D3D11_CULL_MODE& CullMode,
		const DXGI_SAMPLE_DESC& SampleDesc,
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>& RasterizerState
	);

	void CreateDepthStencilState(
		const BOOL& DepthEnable,
		const D3D11_COMPARISON_FUNC& DepthComparisonFunc,
		const BOOL& StencilEnable,
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>& DepthStencilState
	);

	// 1개일 경우 사용
	void CreateBlendState(
		const BOOL& AlphaToCoverageEnable,
		const BOOL& IndependentBlendEnable,
		Microsoft::WRL::ComPtr<ID3D11BlendState>& BlendState,
		const size_t RenderTargetIndex,
		const BOOL& BlendEnable,
		const D3D11_BLEND& SrcBlend,
		const D3D11_BLEND& DestBlend,
		const D3D11_BLEND_OP& BlendOp,
		const D3D11_BLEND& SrcBlendAlpha,
		const D3D11_BLEND& DestBlendAlpha,
		const D3D11_BLEND_OP& BlendOpAlpha,
		const D3D11_COLOR_WRITE_ENABLE& RenderTargetWriteMask
	);

	// 2개 이상일 경우 사용
	template<typename ...Args>
	void CreateBlendState(
		const BOOL& AlphaToCoverageEnable,
		const BOOL& IndependentBlendEnable,
		Microsoft::WRL::ComPtr<ID3D11BlendState>& BlendState,
		const size_t RenderTargetIndex,
		const BOOL& BlendEnable,
		const D3D11_BLEND& SrcBlend,
		const D3D11_BLEND& DestBlend,
		const D3D11_BLEND_OP& BlendOp,
		const D3D11_BLEND& SrcBlendAlpha,
		const D3D11_BLEND& DestBlendAlpha,
		const D3D11_BLEND_OP& BlendOpAlpha,
		const D3D11_COLOR_WRITE_ENABLE& RenderTargetWriteMask,
		Args... args
	);

	template<typename ...Args>
	void CreateBlendState(
		D3D11_BLEND_DESC& BlendStateDesc,
		Microsoft::WRL::ComPtr<ID3D11BlendState>& BlendState,
		const size_t RenderTargetIndex,
		const BOOL& BlendEnable,
		const D3D11_BLEND& SrcBlend,
		const D3D11_BLEND& DestBlend,
		const D3D11_BLEND_OP& BlendOp,
		const D3D11_BLEND& SrcBlendAlpha,
		const D3D11_BLEND& DestBlendAlpha,
		const D3D11_BLEND_OP& BlendOpAlpha,
		const D3D11_COLOR_WRITE_ENABLE& RenderTargetWriteMask,
		Args... args
	);

	void CreateBlendState(
		D3D11_BLEND_DESC& BlendStateDesc,
		Microsoft::WRL::ComPtr<ID3D11BlendState>& BlendState,
		const size_t RenderTargetIndex,
		const BOOL& BlendEnable,
		const D3D11_BLEND& SrcBlend,
		const D3D11_BLEND& DestBlend,
		const D3D11_BLEND_OP& BlendOp,
		const D3D11_BLEND& SrcBlendAlpha,
		const D3D11_BLEND& DestBlendAlpha,
		const D3D11_BLEND_OP& BlendOpAlpha,
		const D3D11_COLOR_WRITE_ENABLE& RenderTargetWriteMask
	);

	void CreateSamplerState(
		const D3D11_TEXTURE_ADDRESS_MODE& TextureAddressMode,
		const D3D11_COMPARISON_FUNC& ComparisonFunc,
		const D3D11_FILTER& Filter,
		Microsoft::WRL::ComPtr<ID3D11SamplerState>& SamplerState
	);
};
