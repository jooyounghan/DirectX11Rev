#include "CameraComponent.h"

using namespace DirectX;

CameraComponent::CameraComponent(
	const uint64_t& componentID, XMFLOAT3 position, 
	const uint32_t& width, const uint32_t& height,
	const float& nearZ, const float& farZ, const float& fovAngle
)
	: AComponent(componentID, position), m_nearZ(nearZ), m_farZ(farZ), m_fovAngle(fovAngle),
	m_viewProjBuffer(sizeof(SViewElement), 1)
{
	TopLeftX = 0.f;
	TopLeftY = 0.f;
	Width = static_cast<float>(width);
	Height = static_cast<float>(height);
	MinDepth = 0.f;
	MaxDepth = 1.f;
}

XMMATRIX CameraComponent::GetViewMatrix()
{
	const XMVECTOR quaternion = GetQuaternion();
	XMVECTOR currentForward = XMVector3Rotate(GDefaultForward, quaternion);
	XMVECTOR currentUp = XMVector3Rotate(GDefaultUp, quaternion);
	return XMMatrixLookToLH(m_position, currentForward, currentUp);

}

XMMATRIX CameraComponent::GetProjectionMatrix()
{
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fovAngle), Width / Height, m_nearZ, m_farZ);
}

void CameraComponent::InitEntity(ID3D11Device* device)
{
	AComponent::InitEntity(device);
	m_viewProjBuffer.Initialize(device);
	m_film = new Texture2DInstance<SRVOption, RTVOption, UAVOption>(
		static_cast<uint32_t>(Width), static_cast<uint32_t>(Height), 1, NULL, NULL,
		D3D11_USAGE_DEFAULT, 
		DXGI_FORMAT_R8G8B8A8_UNORM, 
		device
	);
}

void CameraComponent::UpdateEntity(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	AComponent::UpdateEntity(device, deviceContext);
	m_viewElement.m_viewProj = GetViewMatrix() * GetProjectionMatrix();
	m_viewElement.m_invViewProj = XMMatrixInverse(nullptr, m_viewElement.m_viewProj);
	m_viewProjBuffer.Upload(device, deviceContext, sizeof(SViewElement), 1, &m_viewElement);
}

void CameraComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}