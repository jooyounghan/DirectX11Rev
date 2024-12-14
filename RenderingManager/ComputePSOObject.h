#pragma once
#include "ComputeShader.h"
#include "PSOObject.h"

class ComputePSOObject : public PSOObject
{
public:
	ComputePSOObject(
		ID3D11DepthStencilState* depthStencilState,
		ID3D11BlendState* blendState,
		ID3D11RasterizerState* rasterizerState,
		const std::vector<ID3D11SamplerState*>& samplerStates,
		const D3D11_PRIMITIVE_TOPOLOGY& topology,
		ComputeShader* computeShader
	);
	virtual ~ComputePSOObject();

protected:
	ComputeShader* m_computeShader;

public:
	virtual bool IsValidate(const std::vector<RTVOption*>& renderTargetViews) override;
};
