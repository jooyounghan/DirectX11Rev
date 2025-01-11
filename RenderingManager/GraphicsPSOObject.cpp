#include "GraphicsPSOObject.h"
#include "AShader.h"

GraphicsPSOObject::GraphicsPSOObject(
	AShader* const vs, 
	AShader* const ps, 
	AShader* const hs, 
	AShader* const ds, 
	AShader* const gs, 
	ID3D11RasterizerState* const rasterizerState,
	ID3D11DepthStencilState* const depthStencilState,
	const std::vector<ID3D11SamplerState*>& samplerStates
)
	: m_vertexShader(vs), m_pixelShader(ps), m_hullShader(hs), m_domainShader(ds), m_geometryShader(gs),
	m_rasterizerState(rasterizerState), m_depthStencilState(depthStencilState),
	m_samplerStates(samplerStates)
{
}

void GraphicsPSOObject::ApplyPSOObject(ID3D11DeviceContext* const deviceContext) const
{
	if(m_vertexShader) m_vertexShader->SetShader(deviceContext);
	if (m_pixelShader) m_pixelShader->SetShader(deviceContext);
	if (m_hullShader) m_hullShader->SetShader(deviceContext);
	if (m_domainShader) m_domainShader->SetShader(deviceContext);
	if (m_geometryShader) m_geometryShader->SetShader(deviceContext);
	
	deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);
	deviceContext->RSSetState(m_rasterizerState);
	deviceContext->PSSetSamplers(0, static_cast<UINT>(m_samplerStates.size()), m_samplerStates.data());
}


