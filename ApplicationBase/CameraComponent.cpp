#include "CameraComponent.h"
#include "DynamicBuffer.h"

using namespace std;
using namespace DirectX;

SCameraEntity::SCameraEntity(const uint32_t& width, const uint32_t& height, const float& focalDistance, const float& aperture, const float& nearZ, const float& farZ)
	: m_width(width), m_height(height), m_focalDistance(focalDistance), m_aperture(aperture), m_nearZ(nearZ), m_farZ(farZ)
{
}

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
	m_fovAngle(fovAngle), 
	m_viewEntityBuffer(sizeof(SViewEntity), 1, &m_viewEntity),
	m_cameraEntity(width, height, 100.f, 1.4f, nearZ, farZ),
	m_cameraEntityBuffer(sizeof(SCameraEntity), 1, &m_cameraEntity),
	m_film(width, height, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM),
	m_filteredFilm(width, height, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM),
	m_idFilm(width, height, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT),
	m_idStagingFilm(1, 1, 1, 1, D3D11_CPU_ACCESS_READ, NULL, D3D11_USAGE_STAGING, DXGI_FORMAT_R32_UINT),
	m_depthStencilView(width, height, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R24G8_TYPELESS)
{
	ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);
	m_viewport.MaxDepth = 1.f;
}

void CameraComponent::SetCameraEntity(
	const float& focalDistance, const float& aperture, 
	const float& nearZ, const float& farZ
)
{
	m_cameraEntity.m_focalDistance = focalDistance;
	m_cameraEntity.m_aperture = aperture;
	m_cameraEntity.m_nearZ = nearZ;
	m_cameraEntity.m_farZ = farZ;
	m_isCameraEntityUpdated.SetFlag(true);
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
	m_projMatrix = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fovAngle), 
		m_viewport.Width / m_viewport.Height, 
		m_cameraEntity.m_nearZ, 
		m_cameraEntity.m_farZ
	);

	m_viewEntity.m_viewProj = m_viewMatrix * m_projMatrix;
	m_viewEntity.m_invViewProj = XMMatrixTranspose(XMMatrixInverse(nullptr, m_viewEntity.m_viewProj));
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

