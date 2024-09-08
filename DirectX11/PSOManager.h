#pragma once
#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <memory>

enum EPSOType : size_t
{
	// Position Only
	R8G8B8A8_BoundingComponent_Solid,
	R8G8B8A8_BoundingComponent_Wireframe,
	R8G8B8A8_Picking_ID_Solid,

	// Static
	R8G8B8A8_Static_Solid,
	R8G8B8A8_Static_Wireframe,

	// Skeletal
	R8G8B8A8_Skeletal_Solid,
	R8G8B8A8_Skeletal_Wireframe,
	PSOTypeCount
};

class PSOObject;
class ARenderer;

class PSOManager
{
public:
	PSOManager();
	~PSOManager();

protected:
	std::unique_ptr<PSOObject> PSOObjects[PSOTypeCount];
	std::unique_ptr<ARenderer> Renderers[PSOTypeCount];

public:
	inline PSOObject* GetPSOObject(EPSOType PsoTypeIn) { return PSOObjects[PsoTypeIn].get(); }
	inline ARenderer* GetRenderers(EPSOType PsoTypeIn) { return Renderers[PsoTypeIn].get(); }

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

private:
	void CreateRenderers();
};
