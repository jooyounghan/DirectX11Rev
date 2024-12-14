
#include "ComputePSOObject.h"

ComputePSOObject::ComputePSOObject(
	ID3D11DepthStencilState* depthStencilState, 
	ID3D11BlendState* blendState, 
	ID3D11RasterizerState* rasterizerState, 
	const std::vector<ID3D11SamplerState*>& samplerStates, 
	const D3D11_PRIMITIVE_TOPOLOGY& topology, 
	ComputeShader* computeShader
)
	: PSOObject(depthStencilState, blendState, rasterizerState, samplerStates, topology, { computeShader }), 
	m_computeShader(computeShader)
{
}

ComputePSOObject::~ComputePSOObject()
{
}

bool ComputePSOObject::IsValidate(const std::vector<RTVOption*>& renderTargetViews)
{
	return true;
}
