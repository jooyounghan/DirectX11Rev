#pragma once
#include "AShader.h"
#include <unordered_map>

template <typename T>
concept ShaderType = std::is_base_of_v<AShader, T>;

class PSOManager
{
protected:
	PSOManager() = default;
	PSOManager(const PSOManager&) = delete;
	PSOManager(PSOManager&&) = delete;

public:
	virtual ~PSOManager();

protected:
	std::unordered_map<size_t, AShader*> m_registeredVertexShaders;
	std::unordered_map<size_t, AShader*> m_registeredPixelShaders;
	std::unordered_map<size_t, AShader*> m_registeredHullShaders;
	std::unordered_map<size_t, AShader*> m_registeredDomainShaders;
	std::unordered_map<size_t, AShader*> m_registeredGeometryShaders;
	std::unordered_map<size_t, AShader*> m_registeredComputeShaders;

protected:
	void RegisterVertexShader(const size_t& shaderID, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescs, const D3D11_PRIMITIVE_TOPOLOGY& topology, const std::wstring& shaderPath, const std::string& entryPoint, const std::string& targetVersion, ID3D11Device* device);
	void RegisterPixelShader(const size_t& shaderID, const UINT& renderTargetCounts, const std::wstring& shaderPath, const std::string& entryPoint, const std::string& targetVersion, ID3D11Device* device);
	void RegisterHullShader(const size_t& shaderID, const std::wstring& shaderPath, const std::string& entryPoint, const std::string& targetVersion, ID3D11Device* device);
	void RegisterDomainShader(const size_t& shaderID, const std::wstring& shaderPath, const std::string& entryPoint, const std::string& targetVersion, ID3D11Device* device);
	void RegisterGeometryShader(const size_t& shaderID, const std::wstring& shaderPath, const std::string& entryPoint, const std::string& targetVersion, ID3D11Device* device);
	void RegisterComputeShader(const size_t& shaderID, const std::wstring& shaderPath, const std::string& entryPoint, const std::string& targetVersion, ID3D11Device* device);

public:
	AShader* GetVertexShader(const size_t& shaderID);
	AShader* GetPixelShader(const size_t& shaderID);
	AShader* GetHullShader(const size_t& shaderID);
	AShader* GetDomainShader(const size_t& shaderID);
	AShader* GetGeometryShader(const size_t& shaderID);
	AShader* GetComputeShader(const size_t& shaderID);

private:
	template<typename ShaderType, typename ...Args>
	AShader* CreateShaderImpl(
		const size_t& shaderID,
		const std::wstring& shaderPath,
		const std::string& entryPoint,
		const std::string& targetVersion,
		ID3D11Device* device,
		Args... args
	);
	void RegisterShader(
		std::unordered_map<size_t, AShader*>& shaderContainer,
		const size_t& shaderID,
		AShader* shader
	);
	AShader* GetShader(
		std::unordered_map<size_t, AShader*>& shaderContainer,
		const size_t& shaderID
	);


protected:
	std::unordered_map<size_t, Microsoft::WRL::ComPtr<ID3D11DepthStencilState>> m_registeredDepthStencilStates;
	std::unordered_map<size_t, Microsoft::WRL::ComPtr<ID3D11BlendState>> m_registeredBlendStates;
	std::unordered_map<size_t, Microsoft::WRL::ComPtr<ID3D11RasterizerState>> m_registeredRasterizerStates;
	std::unordered_map<size_t, Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_registeredSamplerStates;

protected:
	void RegisterDepthStencilState(
		const size_t& stateID,
		ID3D11Device* device,
		const BOOL& depthEnable,
		const D3D11_COMPARISON_FUNC& depthComparisonFunc,
		const BOOL& stencilEnable,
		const D3D11_COMPARISON_FUNC& stenctilComparisonFunc = D3D11_COMPARISON_ALWAYS,
		const D3D11_STENCIL_OP& passOp = D3D11_STENCIL_OP_KEEP,
		const D3D11_STENCIL_OP& failOp = D3D11_STENCIL_OP_KEEP,
		const D3D11_STENCIL_OP& stencilDepthFailOp= D3D11_STENCIL_OP_KEEP
	);
	void RegisterBlendState(
		const size_t& stateID,
		ID3D11Device* device,
		const BOOL& AlphaToCoverageEnable,
		const BOOL& IndependentBlendEnable, 
		const std::vector<D3D11_RENDER_TARGET_BLEND_DESC>& renderTargetBlendDescs
	);
	void RegisterRasterizerState(
		const size_t& stateID,
		ID3D11Device* device,
		const D3D11_FILL_MODE& fillMode,
		const D3D11_CULL_MODE& cullMode,
		const BOOL& frontCounterClockwise,
		const DXGI_SAMPLE_DESC& sampleDesc
	);
	void RegisterSamplerState(
		const size_t& stateID,
		ID3D11Device* device,
		const D3D11_TEXTURE_ADDRESS_MODE& textureAddressModeU,
		const D3D11_TEXTURE_ADDRESS_MODE& textureAddressModeV,
		const D3D11_TEXTURE_ADDRESS_MODE& textureAddressModeW,
		const D3D11_COMPARISON_FUNC& comparisonFunc,
		const D3D11_FILTER& filter
	);

protected:
	ID3D11DepthStencilState* const GetDepthStencilState(const size_t& stateID);
	ID3D11BlendState* const GetBlendState(const size_t& stateID);
	ID3D11RasterizerState* const GetRasterizerState(const size_t& stateID);
	ID3D11SamplerState* const GetSamplerState(const size_t& stateID);
};

