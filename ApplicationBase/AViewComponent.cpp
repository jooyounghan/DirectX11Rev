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
	const float& fovAngle
) 
	: AComponent(componentName, componentID, position, angle, scale),
	AViewEntity(),
	m_fovAngle(fovAngle)
{
	SetViewport(width, height);
}

AViewComponent::~AViewComponent()
{
	delete m_viewProjBuffer;
}

void AViewComponent::SetViewport(const uint32_t& width, const uint32_t& height)
{
	TopLeftX = 0.f;
	TopLeftY = 0.f;
	Width = static_cast<float>(width);
	Height = static_cast<float>(height);
	MinDepth = 0.f;
	MaxDepth = 1.f;
}

DirectX::XMMATRIX AViewComponent::GetViewMatrix() const
{
	const XMVECTOR quaternion = GetAbsoluteRotationQuaternion();
	XMVECTOR currentForward = XMVector3Rotate(GDefaultForward, quaternion);
	XMVECTOR currentUp = XMVector3Rotate(GDefaultUp, quaternion);
	return XMMatrixLookToLH(m_absolutePosition, currentForward, currentUp);
}

void AViewComponent::UpdateViewEntity()
{
	const XMMATRIX viewMatrix = GetViewMatrix();
	const XMMATRIX projectionMatrix = GetProjectionMatrix();

	UpdateViewEntityImpl(viewMatrix, projectionMatrix, m_absolutePosition);

	BoundingFrustum::CreateFromMatrix(*this, projectionMatrix);
	this->Transform(*this, XMMatrixInverse(nullptr, viewMatrix));

}

void AViewComponent::OnCollide(ICollisionAcceptor*)
{
}
