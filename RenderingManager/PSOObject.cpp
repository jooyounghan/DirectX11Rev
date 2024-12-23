
#include "PSOObject.h"

using namespace std;

const FLOAT PSOObject::BlendFactor[4] = { 1.f, 1.f, 1.f, 1.f };

PSOObject::PSOObject(
	ID3D11DepthStencilState* depthStencilState, 
	ID3D11BlendState* blendState, 
	ID3D11RasterizerState* rasterizerState, 
	const vector<ID3D11SamplerState*>& samplerStates,
	const D3D11_PRIMITIVE_TOPOLOGY& topology,
	const vector<AShader*>& shaders
)
	: m_depthStencilState(depthStencilState), m_blendState(blendState),
	m_rasterizerState(rasterizerState), m_samplerStates(samplerStates),
	m_topology(topology), m_shaders(shaders)
{
}

void PSOObject::ApplyPSOObject(
	ID3D11DeviceContext* deviceContext, 
	const vector<RTVOption*>& renderTargetViews, 
	DSVOption* depthStencilView, 
	const D3D11_VIEWPORT* viewport
)
{
	if (IsValidate(renderTargetViews))
	{
		m_renderTargetViews.clear();
		m_resetRenderTargetViews.clear();

		m_renderTargetViews.resize(renderTargetViews.size());
		m_resetRenderTargetViews.resize(renderTargetViews.size(), nullptr);

		for (size_t idx = 0; idx < m_renderTargetViews.size(); ++idx)
		{
			m_renderTargetViews[idx] = renderTargetViews[idx]->GetRTV();
		}

		for (AShader* const shader : m_shaders)
		{
			shader->SetShader(deviceContext);
			shader->SetSamplerState(deviceContext, m_samplerStates);
		}

		deviceContext->IASetPrimitiveTopology(m_topology);

		deviceContext->RSSetState(m_rasterizerState);
		deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);

		deviceContext->OMSetBlendState(m_blendState, BlendFactor, 0xFFFFFFFF);

		deviceContext->RSSetViewports(1, viewport);
		deviceContext->OMSetRenderTargets(static_cast<UINT>(renderTargetViews.size()), m_renderTargetViews.data(), depthStencilView->GetDSV());
	}
	else
	{
		throw exception();
	}
}

void PSOObject::ResetPSOObject(ID3D11DeviceContext* deviceContext) const
{
	for (AShader* const shader : m_shaders)
	{
		shader->ResetShader(deviceContext);
		shader->ResetSamplerState(deviceContext);
	}

	deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED);

	deviceContext->RSSetState(nullptr);
	deviceContext->OMSetDepthStencilState(nullptr, 0);

	const FLOAT BlendFactor[4] = { 1.f, 1.f, 1.f, 1.f };
	deviceContext->OMSetBlendState(nullptr, BlendFactor, 0xFFFFFFFF);

	deviceContext->RSSetViewports(1, nullptr);
	deviceContext->OMSetRenderTargets(static_cast<UINT>(m_resetRenderTargetViews.size()), m_resetRenderTargetViews.data(), nullptr);
}
