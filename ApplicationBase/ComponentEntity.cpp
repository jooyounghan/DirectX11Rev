#include "ComponentEntity.h"
#include "DynamicBuffer.h"
#include "ConstantBuffer.h"

#include <iostream>

using namespace DirectX;

SComponent::SComponent(const uint32_t& componentID)
	: m_componentID(componentID), m_componentColor{1.f, 0.f, 0.f}
{
}

ComponentEntity::ComponentEntity(
	const uint32_t& componentID,
	const DirectX::XMFLOAT3& localPosition,
	const DirectX::XMFLOAT3& localAngle,
	const DirectX::XMFLOAT3& localScale
)
	:
	m_localPosition(XMVectorSet(localPosition.x, localPosition.y, localPosition.z, 0.f)),
	m_localAngle(XMVectorSet(localAngle.x, localAngle.y, localAngle.z, 0.f)),
	m_localScale(XMVectorSet(localScale.x, localScale.y, localScale.z, 0.f)),
	m_transformationBuffer(new DynamicBuffer(sizeof(STransformation), 1, &m_transformation)),
	m_transformation(),
	m_componentConstant(componentID),
	m_componentBuffer(new DynamicBuffer(sizeof(SComponent), 1, &m_componentConstant))
{
}

ComponentEntity::~ComponentEntity()
{
	delete m_transformationBuffer;
	delete m_componentBuffer;
}

XMMATRIX ComponentEntity::GetLocalTranformation()
{
	return XMMatrixAffineTransformation(
		m_localScale,
		XMQuaternionIdentity(),
		GetLocalQuaternion(),
		m_localPosition
	);
}

XMVECTOR ComponentEntity::GetLocalQuaternion()
{
	return XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(XMVectorGetX(m_localAngle)),
		XMConvertToRadians(XMVectorGetY(m_localAngle)),
		XMConvertToRadians(XMVectorGetZ(m_localAngle))
	);
}


