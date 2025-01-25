#pragma once

enum class EComponentPSOVertexShader
{
	NONE,
	BOUNDING_COMPONENT,
	GBUFFER_RESOLVE,
	SCENE,
	STATIC_MESH,
	SKELETAL_MESH
};

enum class EComponentPSOPixelShader
{
	NONE,
	FORWARD_BOUNDING_COMPONENT,
	GBUFFER_RESOLVE,
	FORWARD_SCENE,
	FORWARD_MESH,
	DEFFERED_MESH
};

enum class EComponentPSOHullShader
{
	NONE,
	MESH
};

enum class EComponentPSODomainShader
{
	NONE,
	MESH
};

enum class EComponentPSOGeometryShader
{
	NONE,
};


enum class EComponentPSODeptshStencilState
{
	NONE,
	DEPTH_COMPARE_LESS,
	DEPTH_COMPARE_GREATER,
	DEPTH_COMPARE_LEFSS_STENCIL_REPLACE,
	DEPTH_DONTCARE_STENCIL_REPLACE
};

enum class EComponentPSOBlendState
{
	NONE,
};

enum class EComponentPSORasterizerState
{
	NONE,
	CW_SOLID_SS,
	CW_WIREFRAME_SS,
	CCW_SOLID_SS,
	CCW_WIREFRAME_SS
};

enum class EComponentPSOSamplerState
{
	NONE,
	WRAP,
	CLAMP,
	WRAP_COMPARISON_LESS,
	CLAMP_COMPARISON_LESS,
};

enum class EComopnentGraphicsPSOObject
{
	SCENE,
	STATIC_MESH_FORWARD,
	SKELETAL_MESH_FORWARD,
	STATIC_MESH_DEFFERED,
	SKELETAL_MESH_DEFFERED,
	DEFFERED_GBUFFER_RESOLVE
};