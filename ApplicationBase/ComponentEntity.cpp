#include "ComponentEntity.h"

using namespace DirectX;

ComponentEntity::ComponentEntity(const uint64_t& componentID, XMFLOAT3 position)
	: m_position(XMVectorSet(position.x, position.y, position.z, 1.f)),
	m_angle(XMVectorSet(0.f, 0.f, 0.f, 0.f)), 
	m_scale(XMVectorSet(1.f, 1.f, 1.f, 0.f)),
	m_transformationBuffer(sizeof(SComponentElement), 1),
	m_componentElement(componentID)
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

void ComponentEntity::UpdateEntity(ID3D11DeviceContext* deviceContext)
{
	m_componentElement.m_transformation = GetTranformation();
	m_componentElement.m_invTransformation = XMMatrixInverse(nullptr, m_componentElement.m_transformation);

	m_transformationBuffer.Upload(deviceContext, sizeof(SComponentElement), 1, &m_componentElement);
}
