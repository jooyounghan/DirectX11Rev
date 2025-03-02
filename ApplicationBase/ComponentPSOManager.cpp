#include "ComponentPSOManager.h"
#include "GraphicsPSOObject.h"

using namespace std;

ComponentPSOManager::~ComponentPSOManager()
{
	for (auto& graphicPSOObject : m_graphicPSOObjects)
	{
		delete graphicPSOObject.second;
	}
}

ComponentPSOManager* ComponentPSOManager::GetInstance()
{
	static ComponentPSOManager componentPSOManager;
	return &componentPSOManager;
}

void ComponentPSOManager::InitComopnentPSOManager(ID3D11Device* device)
{
	RegisterShadersForComponent(device);
	RegisterDepthStencilStatesForComponent(device);
	RegisterBlendStatesForComponent(device);
	RegisterRasterizerStatesForComponent(device);
	RegisterSamplerStatesForComponent(device);

	RegisterPSOObjectsForComponent();
}

AShader* const ComponentPSOManager::GetComponentPSOVertexShader(const EComponentPSOVertexShader& vsEnum)
{
	return GetVertexShader(VertexShaderID(vsEnum));
}

AShader* const ComponentPSOManager::GetComponentPSOPixelShader(const EComponentPSOPixelShader&  psEnum)
{
	return GetPixelShader(PixelShaderID(psEnum));
}

AShader* const ComponentPSOManager::GetComponentPSOHullShader(const EComponentPSOHullShader& hsEnum)
{
	return GetHullShader(HullShaderID(hsEnum));
}

AShader* const  ComponentPSOManager::GetComponentPSODomainShader(const EComponentPSODomainShader& dsEnum)
{
	return GetDomainShader(DomainShaderID(dsEnum));
}

AShader* const ComponentPSOManager::GetComponentPSOGeometryShader(const EComponentPSOGeometryShader& gsEnum)
{
	return GetGeometryShader(GeometryShaderID(gsEnum));
}

AShader* const ComponentPSOManager::GetComponentPSOComputeShader(const EComponentPSOComputeShader& csEnum)
{
	return GetComputeShader(ComputeShaderID(csEnum));
}

ID3D11DepthStencilState* const  ComponentPSOManager::GetComponentPSODepthStencilState(const EComponentPSODeptshStencilState& dssEnum)
{
	return GetDepthStencilState(DepthStencilStateID(dssEnum));
}

ID3D11BlendState* const ComponentPSOManager::GetComponentPSOBlendState(const EComponentPSOBlendState& bsEnum)
{
	return GetBlendState(BlendStateStateID(bsEnum));
}

ID3D11RasterizerState* const ComponentPSOManager::GetComponentPSORasterizerState(const EComponentPSORasterizerState& rsEnum)
{
	return GetRasterizerState(RasterizerStateStateID(rsEnum));
}

ID3D11SamplerState* const ComponentPSOManager::GetComponentPSOSamplerState(const EComponentPSOSamplerState& samplerEnum)
{
	return GetSamplerState(SamplerStateStateID(samplerEnum));
}

GraphicsPSOObject* const ComponentPSOManager::GetGraphicsPSOObject(const EComopnentGraphicsPSOObject& graphicsPSOObjectType)
{
	if (m_graphicPSOObjects.find(graphicsPSOObjectType) != m_graphicPSOObjects.end())
	{
		return m_graphicPSOObjects.at(graphicsPSOObjectType);
	}
	return nullptr;
}

void ComponentPSOManager::RegisterPSOObjectsForComponent()
{
	m_graphicPSOObjects[EComopnentGraphicsPSOObject::SCENE] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::SCENE),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::SCENE),
		nullptr,
		nullptr,
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CCW_SOLID_SS), NULL,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::DEBUG_COMPONENT] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::DEBUG_COMPONENT),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::DEBUG_COMPONENT),
		nullptr,
		nullptr,
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_WIREFRAME_SS), NULL,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::STATIC_MESH_FORWARD] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::STATIC_MESH),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::FORWARD_MESH),
		GetComponentPSOHullShader(EComponentPSOHullShader::MESH),
		GetComponentPSODomainShader(EComponentPSODomainShader::MESH),
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS), NULL,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS), 
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP), GetComponentPSOSamplerState(EComponentPSOSamplerState::CLAMP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::SKELETAL_MESH_FORWARD] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::SKELETAL_MESH),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::FORWARD_MESH),
		GetComponentPSOHullShader(EComponentPSOHullShader::MESH),
		GetComponentPSODomainShader(EComponentPSODomainShader::MESH),
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS), NULL,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS), 
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP), GetComponentPSOSamplerState(EComponentPSOSamplerState::CLAMP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::STATIC_MESH_DEFERRED] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::STATIC_MESH),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::Deferred_MESH),
		GetComponentPSOHullShader(EComponentPSOHullShader::MESH),
		GetComponentPSODomainShader(EComponentPSODomainShader::MESH),
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS), 1,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LEFSS_STENCIL_REPLACE),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::SKELETAL_MESH_DEFERRED] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::SKELETAL_MESH),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::Deferred_MESH),
		GetComponentPSOHullShader(EComponentPSOHullShader::MESH),
		GetComponentPSODomainShader(EComponentPSODomainShader::MESH),
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS), 1,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LEFSS_STENCIL_REPLACE),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::DEFERRED_GBUFFER_RESOLVE] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::GBUFFER_RESOLVE),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::GBUFFER_RESOLVE),
		nullptr,
		nullptr,
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS), 1,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_DONTCARE_STENCIL_REPLACE),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP), GetComponentPSOSamplerState(EComponentPSOSamplerState::CLAMP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::STATIC_MESH_SPOT_LIGHT_DEPTH_TEST] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::STATIC_MESH_SPOT_LIGHT_DEPTH_TEST),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS), 1,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::SKELETAL_MESH_SPOT_LIGHT_DEPTH_TEST] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::SKELETAL_MESH_SPOT_LIGHT_DEPTH_TEST),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS), 1,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);
	m_graphicPSOObjects[EComopnentGraphicsPSOObject::STATIC_MESH_POINT_LIGHT_DEPTH_TEST] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::STATIC_MESH_POINT_LIGHT_DEPTH_TEST),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS), 1,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);
	m_graphicPSOObjects[EComopnentGraphicsPSOObject::SKELETAL_MESH_POINT_LIGHT_DEPTH_TEST] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::SKELETAL_MESH_POINT_LIGHT_DEPTH_TEST),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS), 1,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);
}

void ComponentPSOManager::RegisterShadersForComponent(ID3D11Device* device)
{
	RegisterVSForComponent(device);
	RegisterPSForComponent(device);

	RegisterHullShader(HullShaderID(EComponentPSOHullShader::MESH), L"../Shaders/MeshComponentHS.hlsl", "main", "hs_5_0", device);
	RegisterDomainShader(DomainShaderID(EComponentPSODomainShader::MESH), L"../Shaders/MeshComponentDS.hlsl", "main", "ds_5_0", device);
}

void ComponentPSOManager::RegisterDepthStencilStatesForComponent(ID3D11Device* device)
{
	
	RegisterDepthStencilState(DepthStencilStateID(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS), device, TRUE, D3D11_COMPARISON_LESS, FALSE);
	RegisterDepthStencilState(DepthStencilStateID(EComponentPSODeptshStencilState::DEPTH_COMPARE_GREATER), device, TRUE, D3D11_COMPARISON_GREATER, FALSE);
	RegisterDepthStencilState(DepthStencilStateID(EComponentPSODeptshStencilState::DEPTH_COMPARE_LEFSS_STENCIL_REPLACE), device, TRUE, D3D11_COMPARISON_LESS, TRUE, D3D11_COMPARISON_ALWAYS, D3D11_STENCIL_OP_REPLACE);
	RegisterDepthStencilState(DepthStencilStateID(EComponentPSODeptshStencilState::DEPTH_DONTCARE_STENCIL_REPLACE), device, FALSE, D3D11_COMPARISON_ALWAYS, TRUE, D3D11_COMPARISON_EQUAL, D3D11_STENCIL_OP_KEEP);
}

void ComponentPSOManager::RegisterBlendStatesForComponent(ID3D11Device* device)
{

}

void ComponentPSOManager::RegisterRasterizerStatesForComponent(ID3D11Device* device)
{
	DXGI_SAMPLE_DESC singleSampleDesc;
	singleSampleDesc.Count = 1;
	singleSampleDesc.Quality = 0;

	RegisterRasterizerState(RasterizerStateStateID(EComponentPSORasterizerState::CW_SOLID_SS), device, D3D11_FILL_SOLID, D3D11_CULL_BACK, FALSE, singleSampleDesc);
	RegisterRasterizerState(RasterizerStateStateID(EComponentPSORasterizerState::CW_WIREFRAME_SS), device, D3D11_FILL_WIREFRAME, D3D11_CULL_BACK, FALSE, singleSampleDesc);
	RegisterRasterizerState(RasterizerStateStateID(EComponentPSORasterizerState::CCW_SOLID_SS), device, D3D11_FILL_SOLID, D3D11_CULL_BACK, TRUE, singleSampleDesc);
	RegisterRasterizerState(RasterizerStateStateID(EComponentPSORasterizerState::CCW_WIREFRAME_SS), device, D3D11_FILL_WIREFRAME, D3D11_CULL_BACK, TRUE, singleSampleDesc);
}

void ComponentPSOManager::RegisterSamplerStatesForComponent(ID3D11Device* device)
{
	RegisterSamplerState(SamplerStateStateID(EComponentPSOSamplerState::WRAP), device, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_COMPARISON_NEVER, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	RegisterSamplerState(SamplerStateStateID(EComponentPSOSamplerState::CLAMP), device, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_COMPARISON_NEVER, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	RegisterSamplerState(SamplerStateStateID(EComponentPSOSamplerState::WRAP_COMPARISON_LESS), device, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_COMPARISON_LESS, D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);
	RegisterSamplerState(SamplerStateStateID(EComponentPSOSamplerState::CLAMP_COMPARISON_LESS), device, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_COMPARISON_LESS, D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);
}

void ComponentPSOManager::RegisterVSForComponent(ID3D11Device* device)
{
	vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	RegisterVertexShader(VertexShaderID(EComponentPSOVertexShader::DEBUG_COMPONENT), inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/DebugComponentVS.hlsl", "main", "vs_5_0", device);

	inputElementDesc.insert(inputElementDesc.end(),
		{
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		}
		);
	RegisterVertexShader(VertexShaderID(EComponentPSOVertexShader::GBUFFER_RESOLVE), inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/GBufferResolveVS.hlsl", "main", "vs_5_0", device);

	inputElementDesc.insert(inputElementDesc.end(),
		{
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		}
	);
	RegisterVertexShader(VertexShaderID(EComponentPSOVertexShader::SCENE), inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/SceneVS.hlsl", "main", "vs_5_0", device);
	RegisterVertexShader(VertexShaderID(EComponentPSOVertexShader::STATIC_MESH), inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST, L"../Shaders/StaticMeshComponentVS.hlsl", "main", "vs_5_0", device);

	inputElementDesc.insert(inputElementDesc.end(),
		{
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		}
	);
	RegisterVertexShader(VertexShaderID(EComponentPSOVertexShader::SKELETAL_MESH), inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST, L"../Shaders/SkeletalMeshComponentVS.hlsl", "main", "vs_5_0", device);

	inputElementDesc =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	RegisterVertexShader(VertexShaderID(EComponentPSOVertexShader::STATIC_MESH_SPOT_LIGHT_DEPTH_TEST), inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/StaticMeshSpotLightDepthTestVS.hlsl", "main", "vs_5_0", device);
	RegisterVertexShader(VertexShaderID(EComponentPSOVertexShader::STATIC_MESH_POINT_LIGHT_DEPTH_TEST), inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/StaticMeshPointLightDepthTestVS.hlsl", "main", "vs_5_0", device);

	inputElementDesc.insert(inputElementDesc.end(),
		{
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 5, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		}
	);
	RegisterVertexShader(VertexShaderID(EComponentPSOVertexShader::SKELETAL_MESH_SPOT_LIGHT_DEPTH_TEST), inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/SkeletalMeshSpotLightDepthTestVS.hlsl", "main", "vs_5_0", device);
	RegisterVertexShader(VertexShaderID(EComponentPSOVertexShader::SKELETAL_MESH_POINT_LIGHT_DEPTH_TEST), inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/SkeletalMeshPointLightDepthTestVS.hlsl", "main", "vs_5_0", device);
}

void ComponentPSOManager::RegisterPSForComponent(ID3D11Device* device)
{
	RegisterPixelShader(PixelShaderID(EComponentPSOPixelShader::DEBUG_COMPONENT), 2, L"../Shaders/DebugComponentPS.hlsl", "main", "ps_5_0", device);
	RegisterPixelShader(PixelShaderID(EComponentPSOPixelShader::GBUFFER_RESOLVE), 1, L"../Shaders/GBufferResolvePS.hlsl", "main", "ps_5_0", device);
	RegisterPixelShader(PixelShaderID(EComponentPSOPixelShader::SCENE), 1, L"../Shaders/ForwardScenePS.hlsl", "main", "ps_5_0", device);
	RegisterPixelShader(PixelShaderID(EComponentPSOPixelShader::FORWARD_MESH), 2, L"../Shaders/ForwardMeshComponentPS.hlsl", "main", "ps_5_0", device);
	RegisterPixelShader(PixelShaderID(EComponentPSOPixelShader::Deferred_MESH), 6, L"../Shaders/DeferredMeshComponentPS.hlsl", "main", "ps_5_0", device);
}
