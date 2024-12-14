
#include "PixelShader.h"

using namespace std;

PixelShader::PixelShader(const UINT& renderTargetCount)
    : m_renderTargetCount(renderTargetCount)
{
}

void PixelShader::CreateShaderImpl(
	const wstring& shaderPath, 
	const string& entryPoint, 
	const string& targetVersion, 
	ID3D11Device* device
)
{
    device->CreatePixelShader(
        shaderByteCode->GetBufferPointer(),
        shaderByteCode->GetBufferSize(),
        NULL, m_pixelShader.GetAddressOf()
    );
}

void PixelShader::SetShader(ID3D11DeviceContext* deviceContext) const
{
    deviceContext->PSSetShader(m_pixelShader.Get(), NULL, NULL);
}

void PixelShader::SetSamplerState(
    ID3D11DeviceContext* deviceContext,
    const std::vector<ID3D11SamplerState*>& samplerStates
)
{
    AShader::SetSamplerState(deviceContext, samplerStates);
    deviceContext->PSSetSamplers(0, static_cast<UINT>(samplerStates.size()), samplerStates.data());
}

void PixelShader::ResetShader(ID3D11DeviceContext* deviceContext) const
{
    deviceContext->PSSetShader(nullptr, NULL, NULL);
}

void PixelShader::ResetSamplerState(ID3D11DeviceContext* deviceContext) const
{
    deviceContext->PSSetSamplers(0, static_cast<UINT>(m_resetSamplerState.size()), m_resetSamplerState.data());
}
