#pragma once
#include "AShader.h"
#include "RTVOption.h"
#include "DSVOption.h"
#include "AViewable.h"

#include <memory>
#include <vector>

class PSOObject
{
protected:
	PSOObject(
		ID3D11DepthStencilState* depthStencilState,
		ID3D11BlendState* blendState,
		ID3D11RasterizerState* rasterizerState,
		const std::vector<ID3D11SamplerState*>& samplerStates,
		const D3D11_PRIMITIVE_TOPOLOGY& topology, 
		const std::vector<AShader*>& shaders
	);

private:
	static const float BlendFactor[4];

protected:
	std::vector<ID3D11RenderTargetView*> m_renderTargetViews;
	std::vector<ID3D11RenderTargetView*> m_resetRenderTargetViews;

protected:
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11BlendState* m_blendState;
	ID3D11RasterizerState* m_rasterizerState;
	std::vector<ID3D11SamplerState*> m_samplerStates;
	D3D11_PRIMITIVE_TOPOLOGY m_topology;
	std::vector<AShader*> m_shaders;

public:
	void ApplyPSOObject(ID3D11DeviceContext* deviceContext, const std::vector<RTVOption*>& renderTargetViews, DSVOption* depthStencilView, AViewable* viewable);
	void ResetPSOObject(ID3D11DeviceContext* deviceContext) const;

public:
	virtual bool IsValidate(const std::vector<RTVOption*>& renderTargetViews) = 0;
};

