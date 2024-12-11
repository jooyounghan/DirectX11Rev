#pragma once
#include "VertexShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include "PSOObject.h"

class GraphicsPSOObject : public PSOObject
{
public:
	GraphicsPSOObject(
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
	);
	virtual ~GraphicsPSOObject();

protected:
	VertexShader* m_vertexShader;
	PixelShader* m_pixelShader;
	HullShader* m_hullShader;
	DomainShader* m_domainShader;
	GeometryShader* m_geometryShader;

public:
	virtual bool IsValidate(const std::vector<RTVOption*>& renderTargetViews) override;
};

