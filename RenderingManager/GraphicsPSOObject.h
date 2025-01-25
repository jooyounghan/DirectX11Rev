#pragma once
#include <d3d11.h>
#include <vector>

class AShader;

class GraphicsPSOObject
{
public:
	GraphicsPSOObject(
		AShader* const vs,
		AShader* const ps,
		AShader* const hs,
		AShader* const ds,
		AShader* const gs,
		ID3D11RasterizerState* const rasterizerState,
		const uint32_t& stencilValue,
		ID3D11DepthStencilState* const depthStencilState,
		
		const std::vector<ID3D11SamplerState*>& samplerStates
	);
	GraphicsPSOObject(const GraphicsPSOObject&) = default;
	GraphicsPSOObject(GraphicsPSOObject&&) = default;

protected:
	AShader* const m_vertexShader;
	AShader* const m_pixelShader;
	AShader* const m_hullShader;
	AShader* const m_domainShader;
	AShader* const m_geometryShader;

protected:
	ID3D11RasterizerState* const m_rasterizerState;
	ID3D11DepthStencilState* const m_depthStencilState;
	const std::vector<ID3D11SamplerState*> m_samplerStates;

protected:
	uint32_t m_stencilValue;


public:
	void ApplyPSOObject(ID3D11DeviceContext* const deviceContext) const;
};