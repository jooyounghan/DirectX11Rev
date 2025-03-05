#include "SpotLightComponent.h"

#include "SpotLightDepthTestRenderer.h"
#include "RenderControlOption.h"
#include "StructuredBuffer.h"

#include "Texture2DInstance.h"
#include "SRVOption.h"

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
	const uint32_t& lightIdx,
	SLightEntity* lightEntityCached,
	StructuredBuffer* lightEntityCachedBuffer,
	SViewEntity* viewEntityCached,
	StructuredBuffer* viewEntityCachedBuffer,
	const float& fovAngle,
	ID3D11DepthStencilView** depthTestDSVAddressOfCached
)
	: LightComponent(componentName, componentID, localPosition, localAngle, lightPower, fallOffStart, fallOffEnd, spotPower, lightIdx, lightEntityCached, lightEntityCachedBuffer),
	m_viewEntityCached(viewEntityCached), m_viewEntityBufferCached(viewEntityCachedBuffer), m_fovAngle(fovAngle),
	m_viewport{0.f, 0.f, static_cast<FLOAT>(GDefaultShadowMapWidth), static_cast<FLOAT>(GDefaultShadowMapHeight), 0.f, 1.f },
	m_depthTestDSVAddressOfCached(depthTestDSVAddressOfCached)
{

}

void SpotLightComponent::SetFovAngle(const float& fovAngle)
{ 
	m_fovAngle = fovAngle; 

	m_isViewEntityChanged.SetFlag(true);
	m_isDBUpdated.SetFlag(true);
}

void SpotLightComponent::UpdateEntity(ID3D11DeviceContext* deviceContext)
{
	LightComponent::UpdateEntity(deviceContext);
	UpdateViewEntity(deviceContext);
}

void SpotLightComponent::UpdateViewEntity(ID3D11DeviceContext* deviceContext)
{
	const XMVECTOR quaternion = GetAbsoluteRotationQuaternion();
	XMVECTOR currentForward = XMVector3Rotate(GDefaultForward, quaternion);
	XMVECTOR currentUp = XMVector3Rotate(GDefaultUp, quaternion);

	m_viewMatrix = XMMatrixLookToLH(m_absolutePosition, currentForward, currentUp);
	m_projMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fovAngle), m_viewport.Width / m_viewport.Height, GDefaultNearZ, m_lightEntityCached->m_fallOffEnd);

	m_viewEntityCached->m_viewProj = m_viewMatrix * m_projMatrix;
	m_viewEntityCached->m_invViewProj = XMMatrixTranspose(XMMatrixInverse(nullptr, m_viewEntityCached->m_viewProj));
	m_viewEntityCached->m_viewProj = XMMatrixTranspose(m_viewEntityCached->m_viewProj);
	XMStoreFloat3(&m_viewEntityCached->m_viewPosition, m_absolutePosition);

	m_viewEntityBufferCached->GetBufferChangedFlag().SetFlag(true);

	UpdateBoundingProperty();
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

	ID3D11DepthStencilView* depthTestDSVAddress = *m_depthTestDSVAddressOfCached;
	deviceContext->ClearDepthStencilView(depthTestDSVAddress, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	SpotLightDepthTestRenderer depthTestRenderer = SpotLightDepthTestRenderer(
		deviceContext, componentPSOManager,
		m_componentEntityBuffer.GetBuffer(),
		m_viewEntityBufferCached->GetSRV(),
		&m_viewport,
		depthTestDSVAddress
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
