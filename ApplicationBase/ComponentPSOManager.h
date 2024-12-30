#pragma once
#include "PSOManager.h"

class ComponentPSOManager : public PSOManager
{
public:
	ComponentPSOManager(ID3D11Device** deviceAddress);

public:
	void InitComopnentPSOManager();

public:
	enum class EComponentPSOVertexShader
	{
		BOUNDING_COMPONENT,
		GBUFFER_RESOLVE,
		SCENE,
		STATIC_MESH,
		SKELETAL_MESH
	};

	enum class EComponentPSOPixelShader
	{
		FORWARD_BOUNDING_COMPONENT,
		GBUFFER_RESOLVE,
		FORWARD_SCENE,
		FORWARD_MESH,
		DEFFERED_MESH
	};

	enum class EComponentPSOHullShader
	{
		MESH
	};

	enum class EComponentPSODomainShader
	{
		MESH
	};

	enum class EComponentPSODeptshStencilState
	{
		DEPTH_COMPARE_LESS,
		DEPTH_COMPARE_GREATER
	};

	enum class EComponentPSOBlendState
	{

	};

	enum class EComponentPSORasterizerState
	{
		CULLBACK_SOLID_SS,
		CULLBACK_WIREFRAME_SS
	};

	enum class EComponentPSOSamplerState
	{
		WRAP,
		CLAMP,
		WRAP_COMPARISON_LESS,
		CLAMP_COMPARISON_LESS,
	};

public:
	AShader* GetComponentPSOVertexShader(const EComponentPSOVertexShader& vsEnum);
	AShader* GetComponentPSOPixelShader(const EComponentPSOPixelShader& vsEnum);
	AShader* GetComponentPSOHullShader(const EComponentPSOHullShader& vsEnum);
	AShader* GetComponentPSODomainShader(const EComponentPSODomainShader& vsEnum);

public:
	ID3D11DepthStencilState* GetComponentPSODepthStencilState(const EComponentPSODeptshStencilState& dssEnum);
	ID3D11BlendState* GetComponentPSOBlendState(const EComponentPSOBlendState& bsEnum);
	ID3D11RasterizerState* GetComponentPSORasterizerState(const EComponentPSORasterizerState& rsEnum);
	ID3D11SamplerState* GetComponentPSOSamplerState(const EComponentPSOSamplerState& samplerEnum);

private:
	void RegisterShaderForComponent();
	void RegisterDepthStencilStateForComponent();
	void RegisterBlendStateForComponent();
	void RegisterRasterizerStateForComponent();
	void RegisterSamplerStateForComponent();

private:
	void RegisterVSForComponent();
	void RegisterPSForComponent();
};

