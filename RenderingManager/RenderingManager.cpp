
#include "RenderingManager.h"

using namespace std;

void RenderingManager::RegisterGraphicsRenderingOption(
	const string& graphicsOptionName, 
	GraphicsPSOObject* graphicsPsoObject, 
	const vector<RTVOption*> renderTargetViews, 
	DSVOption* depthStencilView, 
	const D3D11_VIEWPORT* viewport
)
{
	if (m_namesToOptions.find(graphicsOptionName) != m_namesToOptions.end())
	{
		SGraphicsPSOOption* optionKey = m_namesToOptions[graphicsOptionName].get();
		if (m_optionsToRenderables.find(optionKey) != m_optionsToRenderables.end())
		{
			m_optionsToRenderables[optionKey].clear();
		}
	}

	m_namesToOptions[graphicsOptionName] = make_unique<SGraphicsPSOOption>();
	m_namesToOptions[graphicsOptionName]->graphicsPsoObject = graphicsPsoObject;
	m_namesToOptions[graphicsOptionName]->renderTargetViews = renderTargetViews;
	m_namesToOptions[graphicsOptionName]->depthStencilView = depthStencilView;
	m_namesToOptions[graphicsOptionName]->viewport = viewport;
}

void RenderingManager::RegisterGraphicsRenderingArgs(const std::string& graphicsOptionName, const SRenderingArgs& renderingArgs)
{
	if (m_namesToOptions.find(graphicsOptionName) != m_namesToOptions.end())
	{
		SGraphicsPSOOption* optionKey = m_namesToOptions[graphicsOptionName].get();
		m_optionsToRenderables[optionKey].emplace_back(renderingArgs);
	}
	else
	{
		throw exception();
	}
}

void RenderingManager::RegisterGraphicsRenderingArgs(const std::string& graphicsOptionName, const SRenderingArgs&& renderingArgs)
{
	if (m_namesToOptions.find(graphicsOptionName) != m_namesToOptions.end())
	{
		SGraphicsPSOOption* optionKey = m_namesToOptions[graphicsOptionName].get();
		m_optionsToRenderables[optionKey].emplace_back(renderingArgs);
	}
	else
	{
		throw exception();
	}
}

void RenderingManager::Render(ID3D11DeviceContext* deviceContext)
{
	for (const auto& optionToRenderingArgs : m_optionsToRenderables)
	{
		SGraphicsPSOOption* const option = optionToRenderingArgs.first;
		const vector<SRenderingArgs>& renderables = optionToRenderingArgs.second;

		option->graphicsPsoObject->ApplyPSOObject(deviceContext, option->renderTargetViews, option->depthStencilView, option->viewport);

		for (const SRenderingArgs& renderingArgs : renderables)
		{			
			IRenderable* const& renderable = renderingArgs.renderable;
			deviceContext->IASetIndexBuffer(renderable->GetIndexBuffer(), renderable->GetIndexFormat(), 0);
			
			const vector<ID3D11Buffer*> vertexBuffers = renderable->GetVertexBuffers();
			const vector<UINT>& strides = renderable->GetStrides();
			const vector<UINT>& offsets = renderable->GetOffsets();

			deviceContext->IASetVertexBuffers(0, static_cast<UINT>(vertexBuffers.size()),
				vertexBuffers.data(),
				strides.data(),
				offsets.data()
			);

			deviceContext->VSSetConstantBuffers(0, renderingArgs.vertextShaderArgs.cSCounts, renderingArgs.vertextShaderArgs.cSs);
			deviceContext->VSSetShaderResources(0, renderingArgs.vertextShaderArgs.sRVCounts, renderingArgs.vertextShaderArgs.SRVs);

			deviceContext->PSSetConstantBuffers(0, renderingArgs.pixelShaderArgs.cSCounts, renderingArgs.pixelShaderArgs.cSs);
			deviceContext->PSSetShaderResources(0, renderingArgs.pixelShaderArgs.sRVCounts, renderingArgs.pixelShaderArgs.SRVs);

			deviceContext->HSSetConstantBuffers(0, renderingArgs.hullShaderArgs.cSCounts, renderingArgs.hullShaderArgs.cSs);
			deviceContext->HSSetShaderResources(0, renderingArgs.hullShaderArgs.sRVCounts, renderingArgs.hullShaderArgs.SRVs);

			deviceContext->DSSetConstantBuffers(0, renderingArgs.domainShaderArgs.cSCounts, renderingArgs.domainShaderArgs.cSs);
			deviceContext->DSSetShaderResources(0, renderingArgs.domainShaderArgs.sRVCounts, renderingArgs.domainShaderArgs.SRVs);

			deviceContext->GSSetConstantBuffers(0, renderingArgs.geometryShaderArgs.cSCounts, renderingArgs.geometryShaderArgs.cSs);
			deviceContext->GSSetShaderResources(0, renderingArgs.geometryShaderArgs.sRVCounts, renderingArgs.geometryShaderArgs.SRVs);

			deviceContext->DrawIndexed(renderingArgs.indexCount, renderingArgs.startIndexLocation, 0);

		}
	}
}
