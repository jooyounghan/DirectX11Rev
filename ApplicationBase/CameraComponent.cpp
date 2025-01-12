#include "CameraComponent.h"
#include "Texture2DInstance.h"
#include "RTVOption.h"
#include "SRVOption.h"
#include "UAVOption.h"
#include "DSVOption.h"
#include "DynamicBuffer.h"

using namespace std;
using namespace DirectX;

CameraComponent::CameraComponent(
	const string& componentName,
	const uint32_t& componentID, 
	const XMFLOAT3& position, 
	const XMFLOAT3& angle,
	const XMFLOAT3& scale
)
	: AComponent(componentName, componentID, position, angle, scale),
	m_viewProjBuffer(new DynamicBuffer(sizeof(SViewElement), 1)), m_nearZ(GDefaultNearZ), m_farZ(GDefaultFarZ), m_fovAngle(GDefaultFovAngle)
{

}

CameraComponent::~CameraComponent()
{
	delete m_viewProjBuffer;
}

void CameraComponent::SetCameraProperties(
	const uint32_t& width, const uint32_t& height, 
	const float& nearZ, const float& farZ, 
	const float& fovAngle
)
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

XMMATRIX CameraComponent::GetViewMatrix()
{
	const XMVECTOR quaternion = GetQuaternion();
	XMVECTOR currentForward = XMVector3Rotate(GDefaultForward, quaternion);
	XMVECTOR currentUp = XMVector3Rotate(GDefaultUp, quaternion);
	return XMMatrixLookToLH(m_absolutePosition, currentForward, currentUp);

}

XMMATRIX CameraComponent::GetProjectionMatrix()
{
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fovAngle), Width / Height, m_nearZ, m_farZ);
}

void CameraComponent::InitEntity(ID3D11Device* device)
{
	AComponent::InitEntity(device);
	m_viewProjBuffer->Initialize(device);

	m_film = new Texture2DInstance<SRVOption, RTVOption, UAVOption>(
		static_cast<uint32_t>(Width), static_cast<uint32_t>(Height), 1, 1, NULL, NULL,
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, 
		device
	);

	m_depthStencilViewBuffer = new Texture2DInstance<DSVOption>(
		static_cast<uint32_t>(Width), static_cast<uint32_t>(Height), 1, 1, NULL, NULL,
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_D24_UNORM_S8_UINT,
		device
	);
}

void CameraComponent::UpdateEntity(ID3D11DeviceContext* deviceContext, const float& deltaTime)
{
	UpdateAbsoluteEntities();
	m_viewElement.m_viewProj = GetViewMatrix() * GetProjectionMatrix();
	m_viewElement.m_invViewProj = XMMatrixInverse(nullptr, m_viewElement.m_viewProj);
	m_viewElement.m_viewProj = XMMatrixTranspose(m_viewElement.m_viewProj);
	m_viewElement.m_viewPosition = XMFLOAT3(m_absolutePosition.m128_f32[0], m_absolutePosition.m128_f32[1], m_absolutePosition.m128_f32[2]);
	m_viewProjBuffer->Upload(deviceContext, sizeof(SViewElement), 1, &m_viewElement);
}

void CameraComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}