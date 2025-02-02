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
	static unordered_map<EComponentPSOVertexShader, string> vertexShaderEnumToString
	{
		{ EComponentPSOVertexShader::SCENE, "SceneVS"},
		{ EComponentPSOVertexShader::DEBUG_COMPONENT, "DebugComponentVS"},
		{ EComponentPSOVertexShader::GBUFFER_RESOLVE, "GBufferResolveVS"},
		{ EComponentPSOVertexShader::STATIC_MESH, "StaticMeshComponentVS"},
		{ EComponentPSOVertexShader::SKELETAL_MESH, "SkeletalMeshComponentVS"},
		{ EComponentPSOVertexShader::STATIC_MESH_DEPTH_TEST, "StaticMeshDepthTestVS"},
		{ EComponentPSOVertexShader::SKELETAL_MESH_DEPTH_TEST, "SkeletalMeshDepthTestVS"}
	};
	
	return GetRegisteredShader(vertexShaderEnumToString[vsEnum]);
}

AShader* const  ComponentPSOManager::GetComponentPSOPixelShader(const EComponentPSOPixelShader&  psEnum)
{
	static unordered_map<EComponentPSOPixelShader, string> pixelShaderEnumToString
	{
		{ EComponentPSOPixelShader::SCENE, "ForwardScenePS"},
		{ EComponentPSOPixelShader::DEBUG_COMPONENT, "DebugComponentPS"},
		{ EComponentPSOPixelShader::GBUFFER_RESOLVE, "GBufferResolvePS"},
		{ EComponentPSOPixelShader::FORWARD_MESH, "ForwardMeshComponentPS"},
		{ EComponentPSOPixelShader::DEFFERED_MESH, "DefferedMeshComponentPS"},
	};

	return GetRegisteredShader(pixelShaderEnumToString[psEnum]);
}

AShader* const  ComponentPSOManager::GetComponentPSOHullShader(const EComponentPSOHullShader& hsEnum)
{
	static unordered_map<EComponentPSOHullShader, string> hullShaderEnumToString
	{
		{ EComponentPSOHullShader::MESH, "MeshComponentHS"}
	};

	return GetRegisteredShader(hullShaderEnumToString[hsEnum]);
}

AShader* const  ComponentPSOManager::GetComponentPSODomainShader(const EComponentPSODomainShader& dsEnum)
{
	static unordered_map<EComponentPSODomainShader, string> domainShaderEnumToString
	{
		{ EComponentPSODomainShader::MESH, "MeshComponentDS"}
	};

	return GetRegisteredShader(domainShaderEnumToString[dsEnum]);
}

ID3D11DepthStencilState* const  ComponentPSOManager::GetComponentPSODepthStencilState(const EComponentPSODeptshStencilState& dssEnum)
{
	static unordered_map<EComponentPSODeptshStencilState, string> depthStencilStateEnumToString
	{
		{ EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS, "DepthCompareLess"},
		{ EComponentPSODeptshStencilState::DEPTH_COMPARE_GREATER, "DepthCompareGreater"},
		{ EComponentPSODeptshStencilState::DEPTH_COMPARE_LEFSS_STENCIL_REPLACE, "DepthCompareLessStencilReplace" },
		{ EComponentPSODeptshStencilState::DEPTH_DONTCARE_STENCIL_REPLACE, "DepthDontcareStencilReplace" }
	};

	return GetDepthStencilState(depthStencilStateEnumToString[dssEnum]);
}

ID3D11BlendState* const ComponentPSOManager::GetComponentPSOBlendState(const EComponentPSOBlendState& bsEnum)
{
	static unordered_map<EComponentPSOBlendState, string> blendStateEnumToString
	{

	};

	return GetBlendState(blendStateEnumToString[bsEnum]);
}

ID3D11RasterizerState* const  ComponentPSOManager::GetComponentPSORasterizerState(const EComponentPSORasterizerState& rsEnum)
{
	static unordered_map<EComponentPSORasterizerState, string> rasterizerStateEnumToString
	{
		{ EComponentPSORasterizerState::CW_SOLID_SS, "ClockWiseSolidSS"},
		{ EComponentPSORasterizerState::CW_WIREFRAME_SS, "ClockWiseWireframeSS"},
		{ EComponentPSORasterizerState::CCW_SOLID_SS, "CounterClockWiseSolidSS" },
		{ EComponentPSORasterizerState::CCW_WIREFRAME_SS, "CounterClockWiseWireframeSS" }
	};

	return GetRasterizerState(rasterizerStateEnumToString[rsEnum]);
}

ID3D11SamplerState* const ComponentPSOManager::GetComponentPSOSamplerState(const EComponentPSOSamplerState& samplerEnum)
{
	static unordered_map<EComponentPSOSamplerState, string> samplerStateEnumToString
	{
		{ EComponentPSOSamplerState::WRAP, "Wrap"},
		{ EComponentPSOSamplerState::CLAMP, "Clamp"},
		{ EComponentPSOSamplerState::WRAP_COMPARISON_LESS, "WrapComparisonLess"},
		{ EComponentPSOSamplerState::CLAMP_COMPARISON_LESS, "ClampComparisonLess"}
	};

	return GetSamplerState(samplerStateEnumToString[samplerEnum]);
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
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::SKELETAL_MESH_FORWARD] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::SKELETAL_MESH),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::FORWARD_MESH),
		GetComponentPSOHullShader(EComponentPSOHullShader::MESH),
		GetComponentPSODomainShader(EComponentPSODomainShader::MESH),
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS), NULL,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS), 
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::STATIC_MESH_DEFFERED] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::STATIC_MESH),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::DEFFERED_MESH),
		GetComponentPSOHullShader(EComponentPSOHullShader::MESH),
		GetComponentPSODomainShader(EComponentPSODomainShader::MESH),
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS), 1,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LEFSS_STENCIL_REPLACE),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::SKELETAL_MESH_DEFFERED] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::SKELETAL_MESH),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::DEFFERED_MESH),
		GetComponentPSOHullShader(EComponentPSOHullShader::MESH),
		GetComponentPSODomainShader(EComponentPSODomainShader::MESH),
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS), 1,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LEFSS_STENCIL_REPLACE),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::DEFFERED_GBUFFER_RESOLVE] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::GBUFFER_RESOLVE),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::GBUFFER_RESOLVE),
		nullptr,
		nullptr,
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS), 1,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_DONTCARE_STENCIL_REPLACE),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::STATIC_MESH_DEPTH_TEST] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::STATIC_MESH_DEPTH_TEST),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS), 1,
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::SKELETAL_MESH_DEPTH_TEST] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::SKELETAL_MESH_DEPTH_TEST),
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

	RegisterHullShader("MeshComponentHS", L"../Shaders/MeshComponentHS.hlsl", "main", "hs_5_0", device);
	RegisterDomainShader("MeshComponentDS", L"../Shaders/MeshComponentDS.hlsl", "main", "ds_5_0", device);
}

void ComponentPSOManager::RegisterDepthStencilStatesForComponent(ID3D11Device* device)
{
	RegisterDepthStencilState("DepthCompareLess", device, TRUE, D3D11_COMPARISON_LESS, FALSE);
	RegisterDepthStencilState("DepthCompareGreater", device, TRUE, D3D11_COMPARISON_GREATER, FALSE);
	RegisterDepthStencilState("DepthCompareLessStencilReplace", device, TRUE, D3D11_COMPARISON_LESS, TRUE, D3D11_COMPARISON_ALWAYS, D3D11_STENCIL_OP_REPLACE);
	RegisterDepthStencilState("DepthDontcareStencilReplace", device, FALSE, D3D11_COMPARISON_ALWAYS, TRUE, D3D11_COMPARISON_EQUAL, D3D11_STENCIL_OP_KEEP);
}

void ComponentPSOManager::RegisterBlendStatesForComponent(ID3D11Device* device)
{


}

void ComponentPSOManager::RegisterRasterizerStatesForComponent(ID3D11Device* device)
{
	DXGI_SAMPLE_DESC singleSampleDesc;
	singleSampleDesc.Count = 1;
	singleSampleDesc.Quality = 0;
	RegisterRasterizerState("ClockWiseSolidSS", device, D3D11_FILL_SOLID, D3D11_CULL_BACK, FALSE, singleSampleDesc);
	RegisterRasterizerState("ClockWiseWireframeSS", device, D3D11_FILL_WIREFRAME, D3D11_CULL_BACK, FALSE, singleSampleDesc);
	RegisterRasterizerState("CounterClockWiseSolidSS", device, D3D11_FILL_SOLID, D3D11_CULL_BACK, TRUE, singleSampleDesc);
	RegisterRasterizerState("CounterClockWiseWireframeSS", device, D3D11_FILL_WIREFRAME, D3D11_CULL_BACK, TRUE, singleSampleDesc);
}

void ComponentPSOManager::RegisterSamplerStatesForComponent(ID3D11Device* device)
{
	RegisterSamplerState("Wrap", device, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_COMPARISON_NEVER, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	RegisterSamplerState("Clamp", device, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_COMPARISON_NEVER, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	RegisterSamplerState("WrapComparisonLess", device, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_COMPARISON_LESS, D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);
	RegisterSamplerState("ClampComparisonLess", device, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_COMPARISON_LESS, D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);
}

void ComponentPSOManager::RegisterVSForComponent(ID3D11Device* device)
{
	vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	RegisterVertexShader("DebugComponentVS", inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/DebugComponentVS.hlsl", "main", "vs_5_0", device);

	inputElementDesc.insert(inputElementDesc.end(),
		{
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		}
		);
	RegisterVertexShader("GBufferResolveVS", inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/GBufferResolveVS.hlsl", "main", "vs_5_0", device);

	inputElementDesc.insert(inputElementDesc.end(),
		{
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		}
	);
	RegisterVertexShader("SceneVS", inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/SceneVS.hlsl", "main", "vs_5_0", device);

	inputElementDesc.insert(inputElementDesc.end(),
		{
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		}
		);
	RegisterVertexShader("StaticMeshComponentVS", inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST, L"../Shaders/StaticMeshComponentVS.hlsl", "main", "vs_5_0", device);

	inputElementDesc.insert(inputElementDesc.end(),
		{
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 5, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		}
	);
	RegisterVertexShader("SkeletalMeshComponentVS", inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST, L"../Shaders/SkeletalMeshComponentVS.hlsl", "main", "vs_5_0", device);

	inputElementDesc =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	RegisterVertexShader("StaticMeshDepthTestVS", inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/StaticMeshDepthTestVS.hlsl", "main", "vs_5_0", device);

	inputElementDesc.insert(inputElementDesc.end(),
		{
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 5, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		}
	);
	RegisterVertexShader("SkeletalMeshDepthTestVS", inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/SkeletalMeshDepthTestVS.hlsl", "main", "vs_5_0", device);
}

void ComponentPSOManager::RegisterPSForComponent(ID3D11Device* device)
{
	RegisterPixelShader("DebugComponentPS", 2, L"../Shaders/DebugComponentPS.hlsl", "main", "ps_5_0", device);
	RegisterPixelShader("GBufferResolvePS", 1, L"../Shaders/GBufferResolvePS.hlsl", "main", "ps_5_0", device);
	RegisterPixelShader("ForwardScenePS", 1, L"../Shaders/ForwardScenePS.hlsl", "main", "ps_5_0", device);
	RegisterPixelShader("ForwardMeshComponentPS", 2, L"../Shaders/ForwardMeshComponentPS.hlsl", "main", "ps_5_0", device);
	RegisterPixelShader("DefferedMeshComponentPS", 6, L"../Shaders/DefferedMeshComponentPS.hlsl", "main", "ps_5_0", device);
}
