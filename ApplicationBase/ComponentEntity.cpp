#include "ComponentEntity.h"

using namespace DirectX;

SComponent::SComponent(const uint32_t& componentID)
	: m_componentID(componentID), m_dummy1(NULL), m_dummy2(NULL), m_dummy3(NULL)
{
}

ComponentEntity::ComponentEntity(
	const uint32_t& componentID, 
	const XMFLOAT3& position, 
	const XMFLOAT3& angle, 
	const XMFLOAT3& scale
)
	: m_position(XMVectorSet(position.x, position.y, position.z, 1.f)),
	m_angle(XMVectorSet(angle.x, angle.y, angle.z, 0.f)),
	m_scale(XMVectorSet(scale.x, scale.y, scale.z, 0.f)),
	m_transformationBuffer(sizeof(STransformation), 1),
	m_transformation(),
	m_componentConstant(componentID),
	m_componentBuffer(sizeof(SComponent), 1, &m_componentBuffer)
{
}

XMMATRIX ComponentEntity::GetTranformation()
{
	return XMMatrixAffineTransformation(
		m_scale,
		XMQuaternionIdentity(),
		GetQuaternion(),
		m_position
	);
}

XMVECTOR ComponentEntity::GetQuaternion()
{
	return XMQuaternionRotationRollPitchYaw(
		XMVectorGetX(m_angle),
		XMVectorGetY(m_angle),
		XMVectorGetZ(m_angle)
	);
}

void ComponentEntity::InitEntity(ID3D11Device* device)
{
	m_transformationBuffer.Initialize(device);
}

void ComponentEntity::UpdateEntity(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_transformation.m_transformation = GetTranformation();
	m_transformation.m_invTransformation = XMMatrixInverse(nullptr, m_transformation.m_transformation);

	m_transformationBuffer.Upload(device, deviceContext, sizeof(STransformation), 1, &m_transformation);
}

