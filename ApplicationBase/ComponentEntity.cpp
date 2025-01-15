#include "ComponentEntity.h"
#include "DynamicBuffer.h"
#include "ConstantBuffer.h"

#include <iostream>

using namespace DirectX;

SComponent::SComponent(const uint32_t& componentID)
	: m_componentID(componentID), m_dummy1(NULL), m_dummy2(NULL), m_dummy3(NULL)
{
}

ComponentEntity::ComponentEntity(const uint32_t& componentID)
	: m_transformationBuffer(new DynamicBuffer(sizeof(STransformation), 1, &m_transformation)),
	m_transformation(),
	m_componentConstant(componentID),
	m_componentBuffer(new ConstantBuffer(sizeof(SComponent), 1, &m_componentConstant))
{
}

ComponentEntity::~ComponentEntity()
{
	delete m_transformationBuffer;
	delete m_componentBuffer;
}

XMMATRIX ComponentEntity::GetTranformation()
{
	return XMMatrixAffineTransformation(
		m_absoluteScale,
		XMQuaternionIdentity(),
		GetQuaternion(),
		m_absolutePosition
	);
}

XMVECTOR ComponentEntity::GetQuaternion()
{
	return XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(XMVectorGetX(m_absoluteAngle)),
		XMConvertToRadians(XMVectorGetY(m_absoluteAngle)),
		XMConvertToRadians(XMVectorGetZ(m_absoluteAngle))
	);
}

void ComponentEntity::UpdateComponentTransformation()
{
	m_transformation.m_transformation = GetTranformation();
	m_transformation.m_invTransformation = XMMatrixInverse(nullptr, m_transformation.m_transformation);
	m_transformation.m_transformation = XMMatrixTranspose(m_transformation.m_transformation);
}

