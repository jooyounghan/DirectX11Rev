
#include "HullShader.h"

using namespace std;

void HullShader::CreateShaderImpl(
	const wstring& shaderPath, 
	const string& entryPoint, 
	const string& targetVersion, 
	ID3D11Device* device
)
{
	device->CreateHullShader(
		shaderByteCode->GetBufferPointer(),
		shaderByteCode->GetBufferSize(),
		NULL, m_hullShader.GetAddressOf()
	);
}

void HullShader::SetShader(ID3D11DeviceContext* deviceContext) const
{
	deviceContext->HSSetShader(m_hullShader.Get(), NULL, NULL);
}

void HullShader::SetSamplerState(
	ID3D11DeviceContext* deviceContext,
	const std::vector<ID3D11SamplerState*>& samplerStates
)
{
	AShader::SetSamplerState(deviceContext, samplerStates);
	deviceContext->HSSetSamplers(0, static_cast<UINT>(samplerStates.size()), samplerStates.data());
}

void HullShader::ResetShader(ID3D11DeviceContext* deviceContext) const
{
	deviceContext->HSSetShader(nullptr, NULL, NULL);
}

void HullShader::ResetSamplerState(ID3D11DeviceContext* deviceContext) const
{
	deviceContext->HSSetSamplers(0, static_cast<UINT>(m_resetSamplerState.size()), m_resetSamplerState.data());
}
