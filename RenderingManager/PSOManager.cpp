
#include "PSOManager.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "GeometryShader.h"
#include "ComputeShader.h"

#include "DirectXUtilities.h"


using namespace std;
using namespace Microsoft::WRL;

PSOManager::PSOManager(ID3D11Device** deviceAddress) : m_deviceAddressCached(deviceAddress)
{
}

void PSOManager::RegisterVertexShader(const string& shaderName, const vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescs, const D3D11_PRIMITIVE_TOPOLOGY& topology, const wstring& shaderPath, const string& entryPoint, const string& targetVersion, ID3D11Device* device)
{
	RegisterShaderImpl<VertexShader>(shaderName, shaderPath, entryPoint, targetVersion, device, inputElementDescs, topology);
}

void PSOManager::RegisterPixelShader(const string& shaderName, const UINT& renderTargetCounts, const wstring& shaderPath, const string& entryPoint, const string& targetVersion, ID3D11Device* device)
{
	RegisterShaderImpl<PixelShader>(shaderName, shaderPath, entryPoint, targetVersion, device, renderTargetCounts);
}

void PSOManager::RegisterHullShader(const string& shaderName, const wstring& shaderPath, const string& entryPoint, const string& targetVersion, ID3D11Device* device)
{
	RegisterShaderImpl<HullShader>(shaderName, shaderPath, entryPoint, targetVersion, device);
}

void PSOManager::RegisterDomainShader(const string& shaderName, const wstring& shaderPath, const string& entryPoint, const string& targetVersion, ID3D11Device* device)
{
	RegisterShaderImpl<DomainShader>(shaderName, shaderPath, entryPoint, targetVersion, device);
}

void PSOManager::RegisterGeometryShader(const string& shaderName, const wstring& shaderPath, const string& entryPoint, const string& targetVersion, ID3D11Device* device)
{
	RegisterShaderImpl<GeometryShader>(shaderName, shaderPath, entryPoint, targetVersion, device);
}

void PSOManager::RegisterComputeShader(const string& shaderName, const wstring& shaderPath, const string& entryPoint, const string& targetVersion, ID3D11Device* device)
{
	RegisterShaderImpl<ComputeShader>(shaderName, shaderPath, entryPoint, targetVersion, device);
}

template<typename ShaderType, typename ...Args>
inline void PSOManager::RegisterShaderImpl(
	const string& shaderName, 
	const wstring& shaderPath, 
	const string& entryPoint, 
	const string& targetVersion, 
	ID3D11Device* device,
	Args... args
)
{
	m_registeredShaders[shaderName] = make_unique<ShaderType>(args...);
	m_registeredShaders[shaderName]->CreateShader(shaderPath, entryPoint, targetVersion, device);
}


AShader* const PSOManager::GetRegisteredShader(const string& shaderName)
{
	if (m_registeredShaders.find(shaderName) != m_registeredShaders.end())
	{
		return m_registeredShaders[shaderName].get();
	}
	return nullptr;
}

void PSOManager::RegisterDepthStencilState(
	const string& stateName, 
	const BOOL& depthEnable,
	const D3D11_COMPARISON_FUNC& depthComparisonFunc,
	const BOOL& stencilEnable
)
{
	ComPtr<ID3D11DepthStencilState> depthStencilState;

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	AutoZeroMemory(depthStencilStateDesc);
	depthStencilStateDesc.DepthEnable = depthEnable;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = depthComparisonFunc;
	depthStencilStateDesc.StencilEnable = stencilEnable;
	(*m_deviceAddressCached)->CreateDepthStencilState(&depthStencilStateDesc, depthStencilState.GetAddressOf());

	m_registeredDepthStencilStates[stateName] = depthStencilState;
}

void PSOManager::RegisterBlendState(
	const string& stateName, 
	const BOOL& AlphaToCoverageEnable, 
	const BOOL& IndependentBlendEnable, 
	const vector<D3D11_RENDER_TARGET_BLEND_DESC>& renderTargetBlendDescs
)
{
	ComPtr<ID3D11BlendState> blendState;

	D3D11_BLEND_DESC blendStateDesc;
	AutoZeroMemory(blendStateDesc);
	blendStateDesc.AlphaToCoverageEnable = AlphaToCoverageEnable;
	blendStateDesc.IndependentBlendEnable = IndependentBlendEnable;
	memcpy(
		blendStateDesc.RenderTarget, 
		renderTargetBlendDescs.data(), 
		(renderTargetBlendDescs.size() < 8 ? renderTargetBlendDescs.size() : 8) * sizeof(D3D11_RENDER_TARGET_BLEND_DESC)
	);
	(*m_deviceAddressCached)->CreateBlendState(&blendStateDesc, blendState.GetAddressOf());

	m_registeredBlendStates[stateName] = blendState;
}

void PSOManager::RegisterRasterizerState(
	const string& stateName, 
	const D3D11_FILL_MODE& fillMode, 
	const D3D11_CULL_MODE& cullMode, 
	const DXGI_SAMPLE_DESC& sampleDesc
)
{
	ComPtr<ID3D11RasterizerState> rasterizerState;

	D3D11_RASTERIZER_DESC rasterizerStateDesc;
	AutoZeroMemory(rasterizerStateDesc);
	rasterizerStateDesc.FillMode = fillMode;
	rasterizerStateDesc.CullMode = cullMode;
	rasterizerStateDesc.FrontCounterClockwise = FALSE;
	rasterizerStateDesc.DepthClipEnable = TRUE;
	rasterizerStateDesc.MultisampleEnable = sampleDesc.Count > 1 ? TRUE : FALSE;
	rasterizerStateDesc.AntialiasedLineEnable = FALSE;
	(*m_deviceAddressCached)->CreateRasterizerState(&rasterizerStateDesc, rasterizerState.GetAddressOf());

	m_registeredRasterizerStates[stateName] = rasterizerState;
}

void PSOManager::RegisterSamplerState(
	const string& stateName, 
	const D3D11_TEXTURE_ADDRESS_MODE& textureAddressModeU,
	const D3D11_TEXTURE_ADDRESS_MODE& textureAddressModeV,
	const D3D11_TEXTURE_ADDRESS_MODE& textureAddressModeW,
	const D3D11_COMPARISON_FUNC& comparisonFunc, 
	const D3D11_FILTER& filter
)
{
	ComPtr<ID3D11SamplerState> samplerState;

	D3D11_SAMPLER_DESC samplerDesc;
	AutoZeroMemory(samplerDesc);
	samplerDesc.AddressU = textureAddressModeU;
	samplerDesc.AddressV = textureAddressModeV;
	samplerDesc.AddressW = textureAddressModeW;
	samplerDesc.ComparisonFunc = comparisonFunc;
	samplerDesc.Filter = filter;
	(*m_deviceAddressCached)->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf());

	m_registeredSamplerStates[stateName] = samplerState;
}

ID3D11DepthStencilState* const PSOManager::GetDepthStencilState(const string& stateName)
{
	if (m_registeredDepthStencilStates.find(stateName) != m_registeredDepthStencilStates.end())
	{
		return m_registeredDepthStencilStates[stateName].Get();
	}
	return nullptr;
}

ID3D11BlendState* const PSOManager::GetBlendState(const string& stateName)
{
	if (m_registeredBlendStates.find(stateName) != m_registeredBlendStates.end())
	{
		return m_registeredBlendStates[stateName].Get();
	}
	return nullptr;
}

ID3D11RasterizerState* const PSOManager::GetRasterizerState(const string& stateName)
{
	if (m_registeredRasterizerStates.find(stateName) != m_registeredRasterizerStates.end())
	{
		return m_registeredRasterizerStates[stateName].Get();
	}
	return nullptr;
}

ID3D11SamplerState* const PSOManager::GetSamplerState(const string& stateName)
{
	if (m_registeredSamplerStates.find(stateName) != m_registeredSamplerStates.end())
	{
		return m_registeredSamplerStates[stateName].Get();
	}
	return nullptr;
}
