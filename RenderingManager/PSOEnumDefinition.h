#pragma once

enum class EComponentPSOVertexShader
{
	NONE,
	SCENE,
	DEBUG_COMPONENT,
	GBUFFER_RESOLVE,
	STATIC_MESH,
	SKELETAL_MESH,
	STATIC_MESH_DEPTH_TEST,
	SKELETAL_MESH_DEPTH_TEST,
};

class VertexShaderID
{
public:
	constexpr VertexShaderID(EComponentPSOVertexShader shaderType) : m_shaderType(shaderType) {}

private:
	EComponentPSOVertexShader m_shaderType;

public:
	constexpr operator size_t() const
	{
		return static_cast<size_t>(m_shaderType);
	}
};

enum class EComponentPSOPixelShader
{
	NONE,
	SCENE,
	DEBUG_COMPONENT,
	GBUFFER_RESOLVE,
	FORWARD_MESH,
	Deferred_MESH
};

class PixelShaderID
{
public:
	constexpr PixelShaderID(EComponentPSOPixelShader shaderType) : m_shaderType(shaderType) {}

private:
	EComponentPSOPixelShader m_shaderType;

public:
	constexpr operator size_t() const
	{
		return static_cast<size_t>(m_shaderType);
	}
};

enum class EComponentPSOHullShader
{
	NONE,
	MESH
};

class HullShaderID
{
public:
	constexpr HullShaderID(EComponentPSOHullShader shaderType) : m_shaderType(shaderType) {}

private:
	EComponentPSOHullShader m_shaderType;

public:
	constexpr operator size_t() const
	{
		return static_cast<size_t>(m_shaderType);
	}
};

enum class EComponentPSODomainShader
{
	NONE,
	MESH
};

class DomainShaderID
{
public:
	constexpr DomainShaderID(EComponentPSODomainShader shaderType) : m_shaderType(shaderType) {}

private:
	EComponentPSODomainShader m_shaderType;

public:
	constexpr operator size_t() const
	{
		return static_cast<size_t>(m_shaderType);
	}
};

enum class EComponentPSOGeometryShader
{
	NONE,
};

class GeometryShaderID
{
public:
	constexpr GeometryShaderID(EComponentPSOGeometryShader shaderType) : m_shaderType(shaderType) {}

private:
	EComponentPSOGeometryShader m_shaderType;

public:
	constexpr operator size_t() const
	{
		return static_cast<size_t>(m_shaderType);
	}
};

enum class EComponentPSOComputeShader
{
	NONE,
};

class ComputeShaderID
{
public:
	constexpr ComputeShaderID(EComponentPSOComputeShader shaderType) : m_shaderType(shaderType) {}

private:
	EComponentPSOComputeShader m_shaderType;

public:
	constexpr operator size_t() const
	{
		return static_cast<size_t>(m_shaderType);
	}
};

enum class EComponentPSODeptshStencilState
{
	NONE,
	DEPTH_COMPARE_LESS,
	DEPTH_COMPARE_GREATER,
	DEPTH_COMPARE_LEFSS_STENCIL_REPLACE,
	DEPTH_DONTCARE_STENCIL_REPLACE
};

class DepthStencilStateID
{
public:
	constexpr DepthStencilStateID(EComponentPSODeptshStencilState stateType) : m_stateType(stateType) {}

private:
	EComponentPSODeptshStencilState m_stateType;

public:
	constexpr operator size_t() const
	{
		return static_cast<size_t>(m_stateType);
	}
};

enum class EComponentPSOBlendState
{
	NONE,
};

class BlendStateStateID
{
public:
	constexpr BlendStateStateID(EComponentPSOBlendState stateType) : m_stateType(stateType) {}

private:
	EComponentPSOBlendState m_stateType;

public:
	constexpr operator size_t() const
	{
		return static_cast<size_t>(m_stateType);
	}
};

enum class EComponentPSORasterizerState
{
	NONE,
	CW_SOLID_SS,
	CW_WIREFRAME_SS,
	CCW_SOLID_SS,
	CCW_WIREFRAME_SS
};

class RasterizerStateStateID
{
public:
	constexpr RasterizerStateStateID(EComponentPSORasterizerState stateType) : m_stateType(stateType) {}

private:
	EComponentPSORasterizerState m_stateType;

public:
	constexpr operator size_t() const
	{
		return static_cast<size_t>(m_stateType);
	}
};


enum class EComponentPSOSamplerState
{
	NONE,
	WRAP,
	CLAMP,
	WRAP_COMPARISON_LESS,
	CLAMP_COMPARISON_LESS,
};

class SamplerStateStateID
{
public:
	constexpr SamplerStateStateID(EComponentPSOSamplerState stateType) : m_stateType(stateType) {}

private:
	EComponentPSOSamplerState m_stateType;

public:
	constexpr operator size_t() const
	{
		return static_cast<size_t>(m_stateType);
	}
};

enum class EComopnentGraphicsPSOObject
{
	SCENE,
	DEBUG_COMPONENT,
	STATIC_MESH_FORWARD,
	SKELETAL_MESH_FORWARD,
	STATIC_MESH_Deferred,
	SKELETAL_MESH_Deferred,
	Deferred_GBUFFER_RESOLVE,
	STATIC_MESH_DEPTH_TEST,
	SKELETAL_MESH_DEPTH_TEST
};

class PSOObjectID
{
public:
	constexpr PSOObjectID(EComopnentGraphicsPSOObject psoObjectType) : m_psoObjectType(psoObjectType) {}

private:
	EComopnentGraphicsPSOObject m_psoObjectType;

public:
	constexpr operator size_t() const
	{
		return static_cast<size_t>(m_psoObjectType);
	}
};

