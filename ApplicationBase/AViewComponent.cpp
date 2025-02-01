#include "AViewComponent.h"
#include "DynamicBuffer.h"

using namespace std;
using namespace DirectX;

AViewComponent::AViewComponent(
	const string& componentName, 
	const uint32_t& componentID, 
	const XMFLOAT3& position, 
	const XMFLOAT3& angle, 
	const XMFLOAT3& scale, 
	const uint32_t& width, const uint32_t& height, 
	const float& nearZ, const float& farZ, 
	const float& fovAngle
) 
	: AComponent(componentName, componentID, position, angle, scale),
	m_viewProjBuffer(new DynamicBuffer(sizeof(SViewEntity), 1, &m_viewEntity)),
	m_nearZ(nearZ), m_farZ(farZ), m_fovAngle(fovAngle)
{
	SetCameraProperties(width, height, m_nearZ, m_farZ, m_fovAngle);
}

AViewComponent::~AViewComponent()
{
	delete m_viewProjBuffer;
}

void AViewComponent::SetCameraProperties(const uint32_t& width, const uint32_t& height, const float& nearZ, const float& farZ, const float& fovAngle)
{
	m_nearZ = nearZ;
	m_farZ = farZ;
	m_fovAngle = fovAngle;

	TopLeftX = 0.f;
	TopLeftY = 0.f;
	Width = static_cast<float>(width);
	Height = static_cast<float>(height);
	MinDepth = 0.f;
	MaxDepth = 1.f;
}

XMMATRIX AViewComponent::GetViewMatrix()
{
	const XMVECTOR quaternion = GetAbsoluteRotationQuaternion();
	XMVECTOR currentForward = XMVector3Rotate(GDefaultForward, quaternion);
	XMVECTOR currentUp = XMVector3Rotate(GDefaultUp, quaternion);
	return XMMatrixLookToLH(m_absolutePosition, currentForward, currentUp);
}

XMMATRIX AViewComponent::GetProjectionMatrix()
{
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fovAngle), Width / Height, m_nearZ, m_farZ);
}

void AViewComponent::UpdateViewEntity()
{
	UpdateViewEntityImpl(GetViewMatrix(), GetProjectionMatrix());
}

void AViewComponent::UpdateViewEntityImpl(const XMMATRIX& viewMatrix, const XMMATRIX& projectMatrix)
{
	m_viewEntity.m_viewProj = viewMatrix * projectMatrix;
	m_viewEntity.m_invViewProj = XMMatrixInverse(nullptr, m_viewEntity.m_viewProj);
	m_viewEntity.m_viewProj = XMMatrixTranspose(m_viewEntity.m_viewProj);
	XMStoreFloat3(&m_viewEntity.m_viewPosition, m_absolutePosition);
}
