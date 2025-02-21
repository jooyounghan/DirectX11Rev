#include "SpotLightComponent.h"

#include "DepthTestRenderer.h"
#include "RenderControlOption.h"
#include "StructuredBuffer.h"

using namespace std;
using namespace DirectX;

SpotLightComponent::SpotLightComponent(
	const string& componentName, 
	const uint32_t& componentID, 
	const XMFLOAT3& localPosition, 
	const XMFLOAT3& localAngle, 
	const float& lightPower,
	const float& fallOffStart, 
	const float& fallOffEnd,
	const float& spotPower,
	SLightEntity* lightEntityCached,
	StructuredBuffer* lightEntityCachedBuffer,
	SViewEntity* viewEntityCached,
	StructuredBuffer* viewEntityCachedBuffer,
	const float& fovAngle
)
	: LightComponent(componentName, componentID, localPosition, localAngle, lightPower, fallOffStart, fallOffEnd, spotPower,lightEntityCached, lightEntityCachedBuffer),
	m_viewEntityCached(viewEntityCached), m_viewEntityCachedBuffer(viewEntityCachedBuffer), m_fovAngle(fovAngle),
	m_viewport{0.f, 0.f, static_cast<FLOAT>(GDefaultShadowMapWidth), static_cast<FLOAT>(GDefaultShadowMapHeight), 0.f, 1.f },
	m_depthTestView(GDefaultShadowMapWidth, GDefaultShadowMapHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_TYPELESS)
{
}

void SpotLightComponent::UpdateViewEntity()
{
	if (m_lightEntityCached)
	{
		const XMVECTOR quaternion = GetAbsoluteRotationQuaternion();
		XMVECTOR currentForward = XMVector3Rotate(GDefaultForward, quaternion);
		XMVECTOR currentUp = XMVector3Rotate(GDefaultUp, quaternion);

		m_viewMatrix = XMMatrixLookToLH(m_absolutePosition, currentForward, currentUp);
		m_projMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fovAngle), m_viewport.Width / m_viewport.Height, GDefaultNearZ, m_lightEntityCached->m_fallOffEnd);

		m_viewEntityCached->m_viewProj = m_viewMatrix * m_projMatrix;
		m_viewEntityCached->m_invViewProj = XMMatrixInverse(nullptr, m_viewEntityCached->m_viewProj);
		m_viewEntityCached->m_viewProj = XMMatrixTranspose(m_viewEntityCached->m_viewProj);

		XMStoreFloat3(&m_viewEntityCached->m_viewPosition, m_absolutePosition);

		UpdateBoundingProperty();

		SetModifiedOption(GetComponentUpdateOption(ESpotLightComponentUpdateOption::VIEW_ENTITY));
	}
}

void SpotLightComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}

void SpotLightComponent::GenerateShadowMap(
	ID3D11DeviceContext* deviceContext, 
	ComponentPSOManager* componentPSOManager, 
	const vector<AComponent*>& components
)
{
	vector<AComponent*> renderableComponents = RenderControlOption::GetRenderableComponents(m_parentSceneID, this, components);

	deviceContext->ClearDepthStencilView(m_depthTestView.GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	DepthTestRenderer depthTestRenderer = DepthTestRenderer(
		deviceContext, componentPSOManager,
		m_componentEntityBuffer.GetBuffer(),
		m_viewEntityCachedBuffer->GetSRV(),
		m_lightEntityCachedBuffer->GetSRV(),
		&m_viewport,
		m_depthTestView.GetDSV()
	);

	for (AComponent* renderableComponent : renderableComponents)
	{
		renderableComponent->Accept(&depthTestRenderer);
	}
}

void SpotLightComponent::UpdateBoundingProperty()
{
	BoundingFrustum& frustum = *this;
	BoundingFrustum::CreateFromMatrix(frustum, m_projMatrix);
	Transform(frustum, XMMatrixInverse(nullptr, m_viewMatrix));
}

void SpotLightComponent::OnCollide(ICollisionAcceptor*)
{
}
