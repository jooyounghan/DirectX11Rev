
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

PSOManager::~PSOManager()
{
	for(auto& shader : m_registeredVertexShaders)
	{
		delete shader.second;
	}
	for(auto& shader : m_registeredPixelShaders)
	{
		delete shader.second;
	}
	for(auto& shader : m_registeredHullShaders)
	{
		delete shader.second;
	}
	for(auto& shader : m_registeredDomainShaders)
	{
		delete shader.second;
	}		
	for (auto& shader : m_registeredGeometryShaders)
	{
		delete shader.second;
	}
}

void PSOManager::RegisterVertexShader(const size_t& shaderID, const vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescs, const D3D11_PRIMITIVE_TOPOLOGY& topology, const wstring& shaderPath, const string& entryPoint, const string& targetVersion, ID3D11Device* device)
{
	RegisterShader(m_registeredVertexShaders, shaderID,
		CreateShaderImpl<VertexShader>(shaderID, shaderPath, entryPoint, targetVersion, device, inputElementDescs, topology)
	);
}

void PSOManager::RegisterPixelShader(const size_t& shaderID, const UINT& renderTargetCounts, const wstring& shaderPath, const string& entryPoint, const string& targetVersion, ID3D11Device* device)
{
	RegisterShader(m_registeredPixelShaders, shaderID,
		CreateShaderImpl<PixelShader>(shaderID, shaderPath, entryPoint, targetVersion, device, renderTargetCounts)
	);
}

void PSOManager::RegisterHullShader(const size_t& shaderID, const wstring& shaderPath, const string& entryPoint, const string& targetVersion, ID3D11Device* device)
{
	RegisterShader(m_registeredHullShaders, shaderID,
		CreateShaderImpl<HullShader>(shaderID, shaderPath, entryPoint, targetVersion, device)
	);
}

void PSOManager::RegisterDomainShader(const size_t& shaderID, const wstring& shaderPath, const string& entryPoint, const string& targetVersion, ID3D11Device* device)
{
	RegisterShader(m_registeredDomainShaders, shaderID,
		CreateShaderImpl<DomainShader>(shaderID, shaderPath, entryPoint, targetVersion, device)
	);
}

void PSOManager::RegisterGeometryShader(const size_t& shaderID, const wstring& shaderPath, const string& entryPoint, const string& targetVersion, ID3D11Device* device)
{
	RegisterShader(m_registeredGeometryShaders, shaderID,
		CreateShaderImpl<GeometryShader>(shaderID, shaderPath, entryPoint, targetVersion, device)
	);
}

void PSOManager::RegisterComputeShader(const size_t& shaderID, const wstring& shaderPath, const string& entryPoint, const string& targetVersion, ID3D11Device* device)
{
	RegisterShader(m_registeredComputeShaders, shaderID,
		CreateShaderImpl<ComputeShader>(shaderID, shaderPath, entryPoint, targetVersion, device)
	);
}

AShader* PSOManager::GetVertexShader(const size_t& shaderID)
{
	return GetShader(m_registeredVertexShaders, shaderID);
}

AShader* PSOManager::GetPixelShader(const size_t& shaderID)
{
	return GetShader(m_registeredPixelShaders, shaderID);
}

AShader* PSOManager::GetHullShader(const size_t& shaderID)
{
	return GetShader(m_registeredHullShaders, shaderID);
}

AShader* PSOManager::GetDomainShader(const size_t& shaderID)
{
	return GetShader(m_registeredDomainShaders, shaderID);
}

AShader* PSOManager::GetGeometryShader(const size_t& shaderID)
{
	return GetShader(m_registeredGeometryShaders, shaderID);
}

AShader* PSOManager::GetComputeShader(const size_t& shaderID)
{
	return GetShader(m_registeredComputeShaders, shaderID);
}

template<typename ShaderType, typename ...Args>
AShader* PSOManager::CreateShaderImpl(
	const size_t& shaderID,
	const wstring& shaderPath, 
	const string& entryPoint, 
	const string& targetVersion, 
	ID3D11Device* device,
	Args... args
)
{
	AShader* shader = new ShaderType(args...);
	shader->CreateShader(shaderPath, entryPoint, targetVersion, device);
	return shader;
}

void PSOManager::RegisterShader(std::unordered_map<size_t, AShader*>& shaderContainer, const size_t& shaderID, AShader* shader)
{
	if (shaderContainer.find(shaderID) != shaderContainer.end())
	{
		delete shaderContainer[shaderID];
	}
	shaderContainer[shaderID] = shader;
}

AShader* PSOManager::GetShader(std::unordered_map<size_t, AShader*>& shaderContainer, const size_t& shaderID)
{
	if (shaderContainer.find(shaderID) != shaderContainer.end())
	{
		return shaderContainer[shaderID];
	}
	return nullptr;
}

void PSOManager::RegisterDepthStencilState(
	const size_t& stateID,
	ID3D11Device* device,
	const BOOL& depthEnable,
	const D3D11_COMPARISON_FUNC& depthComparisonFunc,
	const BOOL& stencilEnable,
	const D3D11_COMPARISON_FUNC& stenctilComparisonFunc,
	const D3D11_STENCIL_OP& passOp,
	const D3D11_STENCIL_OP& failOp,
	const D3D11_STENCIL_OP& stencilDepthFailOp
)
{
	ComPtr<ID3D11DepthStencilState> depthStencilState;

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	AutoZeroMemory(depthStencilStateDesc);
	depthStencilStateDesc.DepthEnable = depthEnable;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = depthComparisonFunc;
	depthStencilStateDesc.StencilEnable = stencilEnable;

	depthStencilStateDesc.StencilReadMask = 0xFF;
	depthStencilStateDesc.StencilWriteMask = 0xFF;

	depthStencilStateDesc.FrontFace.StencilFunc = stenctilComparisonFunc;
	depthStencilStateDesc.FrontFace.StencilPassOp = passOp;
	depthStencilStateDesc.FrontFace.StencilFailOp = failOp;
	depthStencilStateDesc.FrontFace.StencilDepthFailOp = stencilDepthFailOp;

	depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	device->CreateDepthStencilState(&depthStencilStateDesc, depthStencilState.GetAddressOf());

	m_registeredDepthStencilStates[stateID] = depthStencilState;
}

void PSOManager::RegisterBlendState(
	const size_t& stateID,
	ID3D11Device* device,
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
	device->CreateBlendState(&blendStateDesc, blendState.GetAddressOf());

	m_registeredBlendStates[stateID] = blendState;
}

void PSOManager::RegisterRasterizerState(
	const size_t& stateID,
	ID3D11Device* device,
	const D3D11_FILL_MODE& fillMode, 
	const D3D11_CULL_MODE& cullMode, 
	const BOOL& frontCounterClockwise,
	const DXGI_SAMPLE_DESC& sampleDesc
)
{
	ComPtr<ID3D11RasterizerState> rasterizerState;

	D3D11_RASTERIZER_DESC rasterizerStateDesc;
	AutoZeroMemory(rasterizerStateDesc);
	rasterizerStateDesc.FillMode = fillMode;
	rasterizerStateDesc.CullMode = cullMode;
	rasterizerStateDesc.FrontCounterClockwise = frontCounterClockwise;
	rasterizerStateDesc.DepthClipEnable = TRUE;
	rasterizerStateDesc.MultisampleEnable = sampleDesc.Count > 1 ? TRUE : FALSE;
	rasterizerStateDesc.AntialiasedLineEnable = FALSE;
	device->CreateRasterizerState(&rasterizerStateDesc, rasterizerState.GetAddressOf());

	m_registeredRasterizerStates[stateID] = rasterizerState;
}

void PSOManager::RegisterSamplerState(
	const size_t& stateID,
	ID3D11Device* device,
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
	device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf());

	m_registeredSamplerStates[stateID] = samplerState;
}

ID3D11DepthStencilState* const PSOManager::GetDepthStencilState(const size_t& stateID)
{
	if (m_registeredDepthStencilStates.find(stateID) != m_registeredDepthStencilStates.end())
	{
		return m_registeredDepthStencilStates[stateID].Get();
	}
	return nullptr;
}

ID3D11BlendState* const PSOManager::GetBlendState(const size_t& stateID)
{
	if (m_registeredBlendStates.find(stateID) != m_registeredBlendStates.end())
	{
		return m_registeredBlendStates[stateID].Get();
	}
	return nullptr;
}

ID3D11RasterizerState* const PSOManager::GetRasterizerState(const size_t& stateID)
{
	if (m_registeredRasterizerStates.find(stateID) != m_registeredRasterizerStates.end())
	{
		return m_registeredRasterizerStates[stateID].Get();
	}
	return nullptr;
}

ID3D11SamplerState* const PSOManager::GetSamplerState(const size_t& stateID)
{
	if (m_registeredSamplerStates.find(stateID) != m_registeredSamplerStates.end())
	{
		return m_registeredSamplerStates[stateID].Get();
	}
	return nullptr;
}
