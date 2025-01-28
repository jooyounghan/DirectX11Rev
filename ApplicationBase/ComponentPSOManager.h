#pragma once
#include "PSOManager.h"
#include "PSOEnumDefinition.h"

class GraphicsPSOObject;

class ComponentPSOManager : public PSOManager
{
protected:
	ComponentPSOManager() = default;
	~ComponentPSOManager() override;
	ComponentPSOManager(const ComponentPSOManager&) = delete;
	ComponentPSOManager(ComponentPSOManager&&) = delete;

public:
	static ComponentPSOManager* GetInstance();

public:
	void InitComopnentPSOManager(ID3D11Device* device);

public:
	AShader* const  GetComponentPSOVertexShader(const EComponentPSOVertexShader& vsEnum);
	AShader* const  GetComponentPSOPixelShader(const EComponentPSOPixelShader& vsEnum);
	AShader* const	GetComponentPSOHullShader(const EComponentPSOHullShader& vsEnum);
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
	void RegisterShadersForComponent(ID3D11Device* device);
	void RegisterDepthStencilStatesForComponent(ID3D11Device* device);
	void RegisterBlendStatesForComponent(ID3D11Device* device);
	void RegisterRasterizerStatesForComponent(ID3D11Device* device);
	void RegisterSamplerStatesForComponent(ID3D11Device* device);

private:
	void RegisterVSForComponent(ID3D11Device* device);
	void RegisterPSForComponent(ID3D11Device* device);
};

