#include "ComponentPSOManager.h"
#include "GraphicsPSOObject.h"

using namespace std;

ComponentPSOManager::ComponentPSOManager(ID3D11Device* const* deviceAddress)
	: PSOManager(deviceAddress)
{
}

ComponentPSOManager::~ComponentPSOManager()
{
	for (auto& graphicPSOObject : m_graphicPSOObjects)
	{
		delete graphicPSOObject.second;
	}
}

void ComponentPSOManager::InitComopnentPSOManager()
{
	RegisterShadersForComponent();
	RegisterDepthStencilStatesForComponent();
	RegisterBlendStatesForComponent();
	RegisterRasterizerStatesForComponent();
	RegisterSamplerStatesForComponent();

	RegisterPSOObjectsForComponent();
}

AShader* const ComponentPSOManager::GetComponentPSOVertexShader(const EComponentPSOVertexShader& vsEnum)
{
	static unordered_map<EComponentPSOVertexShader, string> vertexShaderEnumToString
	{
		{ EComponentPSOVertexShader::BOUNDING_COMPONENT, "BoundingComponentVS"},
		{ EComponentPSOVertexShader::GBUFFER_RESOLVE, "GBufferResolveVS"},
		{ EComponentPSOVertexShader::SCENE, "SceneVS"},
		{ EComponentPSOVertexShader::STATIC_MESH, "StaticMeshComponentVS"},
		{ EComponentPSOVertexShader::SKELETAL_MESH, "SkeletalMeshComponentVS"}
	};
	
	return GetRegisteredShader(vertexShaderEnumToString[vsEnum]);
}

AShader* const  ComponentPSOManager::GetComponentPSOPixelShader(const EComponentPSOPixelShader&  psEnum)
{
	static unordered_map<EComponentPSOPixelShader, string> pixelShaderEnumToString
	{
		{ EComponentPSOPixelShader::FORWARD_BOUNDING_COMPONENT, "ForwardBoundingComponentPS"},
		{ EComponentPSOPixelShader::GBUFFER_RESOLVE, "GBufferResolvePS"},
		{ EComponentPSOPixelShader::FORWARD_SCENE, "ForwardScenePS"},
		{ EComponentPSOPixelShader::FORWARD_MESH, "ForwardMeshComponentPS"},
		{ EComponentPSOPixelShader::DEFFERED_MESH, "DefferedMeshComponentPS"}
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
		{ EComponentPSODeptshStencilState::DEPTH_COMPARE_GREATER, "DepthCompareGreater"}
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
		GetComponentPSOPixelShader(EComponentPSOPixelShader::FORWARD_SCENE),
		nullptr,
		nullptr,
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CCW_SOLID_SS),
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::STATIC_MESH_FORWARD] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::STATIC_MESH),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::FORWARD_MESH),
		GetComponentPSOHullShader(EComponentPSOHullShader::MESH),
		GetComponentPSODomainShader(EComponentPSODomainShader::MESH),
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS),
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::SKELETAL_MESH_FORWARD] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::SKELETAL_MESH),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::FORWARD_MESH),
		GetComponentPSOHullShader(EComponentPSOHullShader::MESH),
		GetComponentPSODomainShader(EComponentPSODomainShader::MESH),
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS),
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::STATIC_MESH_DEFFERED] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::STATIC_MESH),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::DEFFERED_MESH),
		GetComponentPSOHullShader(EComponentPSOHullShader::MESH),
		GetComponentPSODomainShader(EComponentPSODomainShader::MESH),
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS),
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);

	m_graphicPSOObjects[EComopnentGraphicsPSOObject::SKELETAL_MESH_DEFFERED] = new GraphicsPSOObject(
		GetComponentPSOVertexShader(EComponentPSOVertexShader::SKELETAL_MESH),
		GetComponentPSOPixelShader(EComponentPSOPixelShader::DEFFERED_MESH),
		GetComponentPSOHullShader(EComponentPSOHullShader::MESH),
		GetComponentPSODomainShader(EComponentPSODomainShader::MESH),
		nullptr,
		GetComponentPSORasterizerState(EComponentPSORasterizerState::CW_SOLID_SS),
		GetComponentPSODepthStencilState(EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS),
		{ GetComponentPSOSamplerState(EComponentPSOSamplerState::WRAP) }
	);
}

void ComponentPSOManager::RegisterShadersForComponent()
{
	RegisterVSForComponent();
	RegisterPSForComponent();

	RegisterHullShader("MeshComponentHS", L"../Shaders/MeshComponentHS.hlsl", "main", "hs_5_0", *m_deviceAddressCached);
	RegisterDomainShader("MeshComponentDS", L"../Shaders/MeshComponentDS.hlsl", "main", "ds_5_0", *m_deviceAddressCached);
}

void ComponentPSOManager::RegisterDepthStencilStatesForComponent()
{
	RegisterDepthStencilState("DepthCompareLess", TRUE, D3D11_COMPARISON_LESS, FALSE);
	RegisterDepthStencilState("DepthCompareGreater", TRUE, D3D11_COMPARISON_GREATER, FALSE);
}

void ComponentPSOManager::RegisterBlendStatesForComponent()
{


}

void ComponentPSOManager::RegisterRasterizerStatesForComponent()
{
	DXGI_SAMPLE_DESC singleSampleDesc;
	singleSampleDesc.Count = 1;
	singleSampleDesc.Quality = 0;
	RegisterRasterizerState("ClockWiseSolidSS", D3D11_FILL_SOLID, D3D11_CULL_BACK, FALSE, singleSampleDesc);
	RegisterRasterizerState("ClockWiseWireframeSS", D3D11_FILL_WIREFRAME, D3D11_CULL_BACK, FALSE, singleSampleDesc);
	RegisterRasterizerState("CounterClockWiseSolidSS", D3D11_FILL_SOLID, D3D11_CULL_BACK, TRUE, singleSampleDesc);
	RegisterRasterizerState("CounterClockWiseWireframeSS", D3D11_FILL_WIREFRAME, D3D11_CULL_BACK, TRUE, singleSampleDesc);
}

void ComponentPSOManager::RegisterSamplerStatesForComponent()
{
	RegisterSamplerState("Wrap", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_COMPARISON_NEVER, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	RegisterSamplerState("Clamp", D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_COMPARISON_NEVER, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	RegisterSamplerState("WrapComparisonLess", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_COMPARISON_LESS, D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);
	RegisterSamplerState("ClampComparisonLess", D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_COMPARISON_LESS, D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);
}

void ComponentPSOManager::RegisterVSForComponent()
{
	vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	RegisterVertexShader("BoundingComponentVS", inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/BoundingComponentVS.hlsl", "main", "vs_5_0", *m_deviceAddressCached);

	inputElementDesc.insert(inputElementDesc.end(),
		{
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		}
		);
	RegisterVertexShader("GBufferResolveVS", inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/GBufferResolveVS.hlsl", "main", "vs_5_0", *m_deviceAddressCached);

	inputElementDesc.insert(inputElementDesc.end(),
		{
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		}
	);
	RegisterVertexShader("SceneVS", inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"../Shaders/SceneVS.hlsl", "main", "vs_5_0", *m_deviceAddressCached);

	inputElementDesc.insert(inputElementDesc.end(),
		{
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		}
		);
	RegisterVertexShader("StaticMeshComponentVS", inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST, L"../Shaders/StaticMeshComponentVS.hlsl", "main", "vs_5_0", *m_deviceAddressCached);

	inputElementDesc.insert(inputElementDesc.end(),
		{
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 5, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		}
	);
	RegisterVertexShader("SkeletalMeshComponentVS", inputElementDesc, D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST, L"../Shaders/SkeletalMeshComponentVS.hlsl", "main", "vs_5_0", *m_deviceAddressCached);
}

void ComponentPSOManager::RegisterPSForComponent()
{
	RegisterPixelShader("ForwardBoundingComponentPS", 2, L"../Shaders/ForwardBoundingComponentPS.hlsl", "main", "ps_5_0", *m_deviceAddressCached);
	RegisterPixelShader("GBufferResolvePS", 1, L"../Shaders/GBufferResolvePS.hlsl", "main", "ps_5_0", *m_deviceAddressCached);
	RegisterPixelShader("ForwardScenePS", 1, L"../Shaders/ForwardScenePS.hlsl", "main", "ps_5_0", *m_deviceAddressCached);
	RegisterPixelShader("ForwardMeshComponentPS", 2, L"../Shaders/ForwardMeshComponentPS.hlsl", "main", "ps_5_0", *m_deviceAddressCached);
	RegisterPixelShader("DefferedMeshComponentPS", 6, L"../Shaders/DefferedMeshComponentPS.hlsl", "main", "ps_5_0", *m_deviceAddressCached);
}
