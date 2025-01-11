#pragma once
#include "PSOManager.h"
#include "PSOEnumDefinition.h"

class GraphicsPSOObject;

class ComponentPSOManager : public PSOManager
{
public:
	ComponentPSOManager(ID3D11Device* const* deviceAddress);
	~ComponentPSOManager() override;

public:
	void InitComopnentPSOManager();

public:
	AShader* const  GetComponentPSOVertexShader(const EComponentPSOVertexShader& vsEnum);
	AShader* const  GetComponentPSOPixelShader(const EComponentPSOPixelShader& vsEnum);
	AShader* const GetComponentPSOHullShader(const EComponentPSOHullShader& vsEnum);
	AShader* const  GetComponentPSODomainShader(const EComponentPSODomainShader& vsEnum);

public:
	ID3D11DepthStencilState* const  GetComponentPSODepthStencilState(const EComponentPSODeptshStencilState& dssEnum);
	ID3D11BlendState* const GetComponentPSOBlendState(const EComponentPSOBlendState& bsEnum);
	ID3D11RasterizerState* const GetComponentPSORasterizerState(const EComponentPSORasterizerState& rsEnum);
	ID3D11SamplerState* const  GetComponentPSOSamplerState(const EComponentPSOSamplerState& samplerEnum);

protected:
	std::unordered_map<EComopnentGraphicsPSOObject, GraphicsPSOObject*> m_graphicPSOObjects;

public:
	GraphicsPSOObject* const GetGraphicsPSOObject(const EComopnentGraphicsPSOObject& graphicsPSOObjectType);

private:
	void RegisterPSOObjectsForComponent();

private:
	void RegisterShadersForComponent();
	void RegisterDepthStencilStatesForComponent();
	void RegisterBlendStatesForComponent();
	void RegisterRasterizerStatesForComponent();
	void RegisterSamplerStatesForComponent();

private:
	void RegisterVSForComponent();
	void RegisterPSForComponent();
};

