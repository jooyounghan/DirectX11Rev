#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <limits>

class ConstantBuffer;
class DynamicBuffer;

struct STransformation
{
	DirectX::XMMATRIX m_transformation = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_invTransformation = DirectX::XMMatrixIdentity();
};

constexpr uint32_t RootID = NULL;
constexpr uint32_t TempID = std::numeric_limits<uint32_t>::max();

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
	ComponentEntity(
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const DirectX::XMFLOAT3& localAngle,
		const DirectX::XMFLOAT3& localScale
	);
	virtual ~ComponentEntity();

protected:
	DirectX::XMVECTOR m_localPosition;
	DirectX::XMVECTOR m_localAngle;
	DirectX::XMVECTOR m_localScale;

protected:
	STransformation m_transformation;
	DynamicBuffer* m_transformationBuffer;

protected:
	SComponent m_componentConstant;
	ConstantBuffer* m_componentBuffer;

public:
	inline const DirectX::XMVECTOR& GetLocalPosition() const { return m_localPosition; }
	inline const DirectX::XMVECTOR& GetLocalAngle() const { return m_localAngle; }
	inline const DirectX::XMVECTOR& GetLocalScale() const { return m_localScale; }

public:
	DynamicBuffer* GetTransformationBuffer() const { return m_transformationBuffer; }
	ConstantBuffer* GetComponentBuffer() const { return m_componentBuffer; }

public:
	inline const uint32_t& GetComponentID() { return m_componentConstant.m_componentID; }

public:
	DirectX::XMMATRIX GetLocalTranformation();
	DirectX::XMVECTOR GetLocalQuaternion();
};

