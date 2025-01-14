#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class ConstantBuffer;
class DynamicBuffer;

struct STransformation
{
	DirectX::XMMATRIX m_transformation = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_invTransformation = DirectX::XMMatrixIdentity();
};

struct SComponent
{
	SComponent(const uint32_t& componentID);
	const uint32_t m_componentID;
	const uint32_t m_dummy1;
	const uint32_t m_dummy2;
	const uint32_t m_dummy3;
};

class ComponentEntity
{
public:
	ComponentEntity(const uint32_t& componentID);
	virtual ~ComponentEntity();

protected:
	DirectX::XMVECTOR m_absolutePosition = DirectX::XMVectorZero();
	DirectX::XMVECTOR m_absoluteAngle = DirectX::XMVectorZero();
	DirectX::XMVECTOR m_absoluteScale = DirectX::XMVectorSet(1.f, 1.f, 1.f, 1.f);

protected:
	STransformation m_transformation;
	DynamicBuffer* m_transformationBuffer;

protected:
	SComponent m_componentConstant;
	ConstantBuffer* m_componentBuffer;

public:
	DynamicBuffer* GetTransformationBuffer() const { return m_transformationBuffer; }
	const ConstantBuffer* GetComponentBuffer() const { return m_componentBuffer; }

public:
	inline const uint32_t& GetComponentID() { return m_componentConstant.m_componentID; }

public:
	DirectX::XMMATRIX GetTranformation();
	DirectX::XMVECTOR GetQuaternion();

public:
	void UpdateComponentTransformation();
};

