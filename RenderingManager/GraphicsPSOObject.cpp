
#include "GraphicsPSOObject.h"

GraphicsPSOObject::GraphicsPSOObject(
	ID3D11DepthStencilState* depthStencilState,
	ID3D11BlendState* blendState,
	ID3D11RasterizerState* rasterizerState,
	const std::vector<ID3D11SamplerState*>& samplerStates,
	const D3D11_PRIMITIVE_TOPOLOGY& topology,
	VertexShader* vertexShader,
	PixelShader* pixelShader,
	HullShader* hullShader,
	DomainShader* domainShader,
	GeometryShader* geometryShader
) : PSOObject(depthStencilState, blendState, rasterizerState, samplerStates, topology, { vertexShader,pixelShader,hullShader,domainShader,geometryShader }),
	m_vertexShader(vertexShader), m_pixelShader(pixelShader), m_hullShader(hullShader), m_domainShader(domainShader), m_geometryShader(geometryShader)
{

}

GraphicsPSOObject::~GraphicsPSOObject()
{
}

bool GraphicsPSOObject::IsValidate(const std::vector<RTVOption*>& renderTargetViews)
{
	if (m_vertexShader == nullptr || m_pixelShader == nullptr)
	{
		return false;
	}
	if (m_pixelShader->GetRenderTargetCounts() != static_cast<UINT>(renderTargetViews.size()))
	{
		return false;
	}

	return true;
}
