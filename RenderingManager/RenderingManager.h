#pragma once
#include "GraphicsPSOObject.h"
#include "ComputeShader.h"

#include "IRenderable.h"
#include <unordered_map>

struct SGraphicsPSOOption
{
	GraphicsPSOObject* graphicsPsoObject;
	std::vector<RTVOption*> renderTargetViews;
	DSVOption* depthStencilView;
	AViewable* viewable;
};

struct SShaderArgs
{
	ID3D11Buffer* const* cSs;
	UINT cSCounts;
	ID3D11ShaderResourceView* const* SRVs;
	UINT sRVCounts;
};

struct SRenderingArgs
{
	IRenderable* renderable;
	UINT indexCount;
	UINT startIndexLocation;
	SShaderArgs vertextShaderArgs;
	SShaderArgs pixelShaderArgs;
	SShaderArgs hullShaderArgs;
	SShaderArgs domainShaderArgs;
	SShaderArgs geometryShaderArgs;
};


class RenderingManager
{
public:
	RenderingManager() = default;

private:
	std::unordered_map<std::string, std::unique_ptr<SGraphicsPSOOption>> m_namesToOptions;
	std::unordered_map<SGraphicsPSOOption*, std::vector<SRenderingArgs>> m_optionsToRenderables;

public:
	void RegisterGraphicsRenderingOption(
		const std::string& graphicsOptionName,
		GraphicsPSOObject* graphicsPsoObject,
		const std::vector<RTVOption*> renderTargetViews,
		DSVOption* depthStencilView,
		AViewable* viewable
	);
	void RegisterGraphicsRenderingArgs(
		const std::string& graphicsOptionName,
		const SRenderingArgs& renderingArgs
	);
	void RegisterGraphicsRenderingArgs(
		const std::string& graphicsOptionName,
		const SRenderingArgs&& renderingArgs
	);

public:
	void Render(ID3D11DeviceContext* deviceContext);
};

