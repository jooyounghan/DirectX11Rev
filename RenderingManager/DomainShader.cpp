
#include "DomainShader.h"

using namespace std;

void DomainShader::CreateShaderImpl(
	const wstring& shaderPath,
	const string& entryPoint, 
	const string& targetVersion, 
	ID3D11Device* device
)
{
	device->CreateDomainShader(
		shaderByteCode->GetBufferPointer(),
		shaderByteCode->GetBufferSize(),
		NULL, m_domainShader.GetAddressOf()
	);
}

void DomainShader::SetShader(ID3D11DeviceContext* deviceContext) const
{
	deviceContext->DSSetShader(m_domainShader.Get(), NULL, NULL);
}

void DomainShader::SetSamplerState(
	ID3D11DeviceContext* deviceContext,
	const std::vector<ID3D11SamplerState*>& samplerStates
)
{
	AShader::SetSamplerState(deviceContext, samplerStates);
	deviceContext->DSSetSamplers(0, static_cast<UINT>(samplerStates.size()), samplerStates.data());
}

void DomainShader::ResetShader(ID3D11DeviceContext* deviceContext) const
{
	deviceContext->GSSetShader(nullptr, NULL, NULL);
}

void DomainShader::ResetSamplerState(ID3D11DeviceContext* deviceContext) const
{
	deviceContext->DSSetSamplers(0, static_cast<UINT>(m_resetSamplerState.size()), m_resetSamplerState.data());
}
