#include "CameraComponent.h"
#include "DynamicBuffer.h"

using namespace std;
using namespace DirectX;

CameraComponent::CameraComponent(
	const string& componentName,
	const uint32_t& componentID, 
	const XMFLOAT3& position, 
	const XMFLOAT3& angle,
	const XMFLOAT3& scale,
	const uint32_t& width, const uint32_t& height, 
	const float& fovAngle,
	const float& nearZ, const float& farZ
)
	: AComponent(componentName, componentID, position, angle, scale),
	m_fovAngle(fovAngle), m_nearZ(nearZ), m_farZ(farZ),
	m_viewEntityBuffer(sizeof(SViewEntity), 1, &m_viewEntity),
	m_film(width, height, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM),
	m_idFilm(width, height, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT),
	m_idStagingFilm(1, 1, 1, 1, D3D11_CPU_ACCESS_READ, NULL, D3D11_USAGE_STAGING, DXGI_FORMAT_R32_UINT),
	m_depthStencilView(width, height, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_D24_UNORM_S8_UINT)
{
	ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);
	m_viewport.MaxDepth = 1.f;
}

void CameraComponent::SetNearZ(const float& nearZ) 
{ 
	m_nearZ = nearZ;	

	m_isViewEntityUpdated.SetFlag(true);
	m_isDBUpdated.SetFlag(true);
}

void CameraComponent::SetFarZ(const float& farZ) 
{ 
	m_farZ = farZ; 

	m_isViewEntityUpdated.SetFlag(true);
	m_isDBUpdated.SetFlag(true);
}

void CameraComponent::SetFovAngle(const float& fovAngle) 
{ 
	m_fovAngle = fovAngle; 

	m_isViewEntityUpdated.SetFlag(true);
	m_isDBUpdated.SetFlag(true);
}

void CameraComponent::SetViewport(const uint32_t& width, const uint32_t& height)
{
	m_viewport.TopLeftX = 0.f;
	m_viewport.TopLeftY = 0.f;
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;

	m_isViewEntityUpdated.SetFlag(true);
	m_isDBUpdated.SetFlag(true);
}

void CameraComponent::UpdateEntity(ID3D11DeviceContext* deviceContext)
{
	AComponent::UpdateEntity(deviceContext);
	UpdateViewEntity(deviceContext);
}

void CameraComponent::UpdateViewEntity(ID3D11DeviceContext* deviceContext)
{
	const XMVECTOR quaternion = GetAbsoluteRotationQuaternion();
	XMVECTOR currentForward = XMVector3Rotate(GDefaultForward, quaternion);
	XMVECTOR currentUp = XMVector3Rotate(GDefaultUp, quaternion);

	m_viewMatrix = XMMatrixLookToLH(m_absolutePosition, currentForward, currentUp);
	m_projMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fovAngle), m_viewport.Width / m_viewport.Height, m_nearZ, m_farZ);

	m_viewEntity.m_viewProj = m_viewMatrix * m_projMatrix;
	m_viewEntity.m_invViewProj = XMMatrixInverse(nullptr, m_viewEntity.m_viewProj);
	m_viewEntity.m_viewProj = XMMatrixTranspose(m_viewEntity.m_viewProj);
	XMStoreFloat3(&m_viewEntity.m_viewPosition, m_absolutePosition);
	m_viewEntityBuffer.Upload(deviceContext);

	UpdateBoundingProperty();
}

void CameraComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}

void CameraComponent::UpdateBoundingProperty()
{
	BoundingFrustum& frustum = *this;
	BoundingFrustum::CreateFromMatrix(frustum, m_projMatrix);
	Transform(frustum, XMMatrixInverse(nullptr, m_viewMatrix));
}

void CameraComponent::OnCollide(ICollisionAcceptor*)
{
}
