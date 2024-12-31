
#include "VertexShader.h"

using namespace std;

VertexShader::VertexShader(
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescs,
    const D3D11_PRIMITIVE_TOPOLOGY& topology
)
	: m_inputElementDescs(inputElementDescs),
    m_topology(topology)
{
}

void VertexShader::CreateShaderImpl(
    const wstring& shaderPath,
    const string& entryPoint,
    const string& targetVersion,
    ID3D11Device* device
)
{
    device->CreateVertexShader(
        shaderByteCode->GetBufferPointer(),
        shaderByteCode->GetBufferSize(),
        NULL, m_vertexShader.GetAddressOf()
    );

    device->CreateInputLayout(
        m_inputElementDescs.data(), static_cast<UINT>(m_inputElementDescs.size()),
        shaderByteCode->GetBufferPointer(),
        shaderByteCode->GetBufferSize(),
        m_inputLayout.GetAddressOf()
    );
}

void VertexShader::SetShader(ID3D11DeviceContext* deviceContext) const
{
    deviceContext->IASetInputLayout(m_inputLayout.Get());
    deviceContext->IASetPrimitiveTopology(m_topology);
    deviceContext->VSSetShader(m_vertexShader.Get(), NULL, NULL);
}

void VertexShader::SetSamplerState(
    ID3D11DeviceContext* deviceContext, 
    const std::vector<ID3D11SamplerState*>& samplerStates
)
{
    AShader::SetSamplerState(deviceContext, samplerStates);
    deviceContext->VSSetSamplers(0, static_cast<UINT>(samplerStates.size()), samplerStates.data());
}

void VertexShader::ResetShader(ID3D11DeviceContext* deviceContext) const
{
    deviceContext->IASetInputLayout(nullptr);
    deviceContext->VSSetShader(nullptr, NULL, NULL);
}

void VertexShader::ResetSamplerState(ID3D11DeviceContext* deviceContext) const
{
    deviceContext->VSSetSamplers(0, static_cast<UINT>(m_resetSamplerState.size()), m_resetSamplerState.data());
}
