#include "GraphicsPSOObject.h"
#include "AShader.h"

GraphicsPSOObject::GraphicsPSOObject(
	AShader* const vs, 
	AShader* const ps, 
	AShader* const hs, 
	AShader* const ds, 
	AShader* const gs, 
	ID3D11RasterizerState* const rasterizerState,
	const uint32_t& stencilValue,
	ID3D11DepthStencilState* const depthStencilState,
	const std::vector<ID3D11SamplerState*>& samplerStates
)
	: m_vertexShader(vs), m_pixelShader(ps), m_hullShader(hs), m_domainShader(ds), m_geometryShader(gs),
	m_rasterizerState(rasterizerState), m_depthStencilState(depthStencilState),
	m_stencilValue(stencilValue), m_samplerStates(samplerStates)
{
}

void GraphicsPSOObject::ApplyPSOObject(ID3D11DeviceContext* const deviceContext) const
{
	m_vertexShader ? m_vertexShader->SetShader(deviceContext) : deviceContext->VSSetShader(nullptr, NULL, NULL);
	m_pixelShader ? m_pixelShader->SetShader(deviceContext) : deviceContext->PSSetShader(nullptr, NULL, NULL);
	m_hullShader ? m_hullShader->SetShader(deviceContext) : deviceContext->HSSetShader(nullptr, NULL, NULL);
	m_domainShader ? m_domainShader->SetShader(deviceContext) : deviceContext->DSSetShader(nullptr, NULL, NULL);
	m_geometryShader ? m_geometryShader->SetShader(deviceContext) : deviceContext->GSSetShader(nullptr, NULL, NULL);
	
	deviceContext->OMSetDepthStencilState(m_depthStencilState, m_stencilValue);
	deviceContext->RSSetState(m_rasterizerState);


	deviceContext->DSSetSamplers(0, static_cast<UINT>(m_samplerStates.size()), m_samplerStates.data());
	deviceContext->PSSetSamplers(0, static_cast<UINT>(m_samplerStates.size()), m_samplerStates.data());
}


