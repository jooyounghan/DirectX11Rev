
#include "GeometryShader.h"

using namespace std;

void GeometryShader::CreateShaderImpl(
	const wstring& shaderPath, 
	const string& entryPoint, 
	const string& targetVersion, 
	ID3D11Device* device
)
{
	device->CreateGeometryShader(
		shaderByteCode->GetBufferPointer(),
		shaderByteCode->GetBufferSize(),
		NULL, m_geometryShader.GetAddressOf()
	);
}

void GeometryShader::SetShader(ID3D11DeviceContext* deviceContext) const
{
	deviceContext->GSSetShader(m_geometryShader.Get(), NULL, NULL);
}

void GeometryShader::SetSamplerState(
	ID3D11DeviceContext* deviceContext,
	const std::vector<ID3D11SamplerState*>& samplerStates
)
{
	AShader::SetSamplerState(deviceContext, samplerStates);
	deviceContext->GSSetSamplers(0, static_cast<UINT>(samplerStates.size()), samplerStates.data());
}

void GeometryShader::ResetShader(ID3D11DeviceContext* deviceContext) const
{
	deviceContext->GSSetShader(nullptr, NULL, NULL);
}

void GeometryShader::ResetSamplerState(ID3D11DeviceContext* deviceContext) const
{
	deviceContext->GSSetSamplers(0, static_cast<UINT>(m_resetSamplerState.size()), m_resetSamplerState.data());
}
