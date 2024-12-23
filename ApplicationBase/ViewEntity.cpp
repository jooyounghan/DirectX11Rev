#include "ViewEntity.h"

using namespace DirectX;


ViewEntity::ViewEntity(
	const uint64_t& componentID, XMFLOAT3 position, 
	const float& width, const float& height, 
	const float& nearZ, const float& farZ, const float& fovAngle
)
	: ComponentEntity(componentID, position), m_nearZ(nearZ), m_farZ(farZ), m_fovAngle(fovAngle),
	m_viewProjBuffer(sizeof(SViewElement), 1)
{
	TopLeftX = 0.f;
	TopLeftY = 0.f;
	Width = width;
	Height = height;
	MinDepth = 0.f;
	MaxDepth = 1.f;
}

XMMATRIX ViewEntity::GetViewMatrix()
{
	const XMVECTOR quaternion = GetQuaternion();
	XMVECTOR currentForward = XMVector3Rotate(GDefaultForward, quaternion);
	XMVECTOR currentUp = XMVector3Rotate(GDefaultUp, quaternion);
	return XMMatrixLookToLH(m_position, currentForward, currentUp);

}

XMMATRIX ViewEntity::GetProjectionMatrix()
{
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fovAngle), Width / Height, m_nearZ, m_farZ);
}

void ViewEntity::InitEntity(ID3D11Device* device)
{
	ComponentEntity::InitEntity(device);
	m_viewProjBuffer.Initialize(device);
}

void ViewEntity::UpdateEntity(ID3D11DeviceContext* deviceContext)
{
	ComponentEntity::UpdateEntity(deviceContext);
	m_viewElement.m_viewProj = GetViewMatrix() * GetProjectionMatrix();
	m_viewElement.m_invViewProj = XMMatrixInverse(nullptr, m_viewElement.m_viewProj);
	m_viewProjBuffer.Upload(deviceContext, sizeof(SViewElement), 1, &m_viewElement);
}
