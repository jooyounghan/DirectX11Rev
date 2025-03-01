#include "PointLightComponent.h"

#include "DepthTestRenderer.h"
#include "RenderControlOption.h"
#include "StructuredBuffer.h"

using namespace std;
using namespace DirectX;

PointLightFrustum::PointLightFrustum(
	const XMVECTOR& absolutePositionCached,
	const XMVECTOR& absoluteRadianOffset,
	const float& fallOffEndCached,
	SViewEntity* viewEntityCached,
	StructuredBuffer* viewEntityBufferCahced,
	ID3D11DepthStencilView** depthTestDSVAddressOfCached
)
	: m_absolutePositionCached(absolutePositionCached), 
	m_absoluteRadianOffset(absoluteRadianOffset),
	m_fallOffEndCached(fallOffEndCached),
	m_viewEntityCached(viewEntityCached),
	m_viewEntityBufferCahced(viewEntityBufferCahced),
	m_depthTestDSVAddressOfCached(depthTestDSVAddressOfCached)
{
}

void PointLightFrustum::UpdateViewEntity()
{	
	const XMVECTOR quaternion = XMQuaternionRotationRollPitchYawFromVector(m_absoluteRadianOffset);
	XMVECTOR currentForward = XMVector3Rotate(GDefaultForward, quaternion);
	XMVECTOR currentUp = XMVector3Rotate(GDefaultUp, quaternion);

	m_viewMatrix = XMMatrixLookToLH(m_absolutePositionCached, currentForward, currentUp);
	m_projMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f), 1.f, GDefaultNearZ, m_fallOffEndCached);

	m_viewEntityCached->m_viewProj = m_viewMatrix * m_projMatrix;
	m_viewEntityCached->m_invViewProj = XMMatrixInverse(nullptr, m_viewEntityCached->m_viewProj);
	m_viewEntityCached->m_viewProj = XMMatrixTranspose(m_viewEntityCached->m_viewProj);

	XMStoreFloat3(&m_viewEntityCached->m_viewPosition, m_absolutePositionCached);

	UpdateBoundingProperty();
}

void PointLightFrustum::UpdateBoundingProperty()
{
	BoundingFrustum::CreateFromMatrix(*this, m_projMatrix);
	this->Transform(*this, XMMatrixInverse(nullptr, m_viewMatrix));
}

void PointLightFrustum::OnCollide(ICollisionAcceptor*)
{
}

PointLightComponent::PointLightComponent(
	const string& componentName,
	const uint32_t& componentID,
	const XMFLOAT3& localPosition,
	const float& lightPower,
	const float& fallOffStart,
	const float& fallOffEnd,
	const uint32_t& lightIdx,
	SLightEntity* lightEntityCached,
	StructuredBuffer* lightEntityCachedBuffer,
	const array<SViewEntity*, 6>& viewEntitiesCached,
	const array<StructuredBuffer*, 6>& viewEntityBuffersCached,
	const std::array<ID3D11DepthStencilView**, 6>& depthTestDSVCubeAddressOfCached
)
	: LightComponent(componentName, componentID, localPosition, XMFLOAT3(0.f, 0.f, 0.f), lightPower, 
		fallOffStart, fallOffEnd, 1.f, lightIdx, lightEntityCached, lightEntityCachedBuffer
	), m_viewport{ 0.f, 0.f, static_cast<FLOAT>(GDefaultShadowMapWidth), static_cast<FLOAT>(GDefaultShadowMapHeight), 0.f, 1.f },
	m_pointLightFrustums{
		PointLightFrustum(m_absolutePosition, XMVectorSet(0.0f, XM_PIDIV2, 0.f, 0.f), lightEntityCached->m_fallOffEnd, viewEntitiesCached.at(0), viewEntityBuffersCached.at(0), depthTestDSVCubeAddressOfCached.at(0)),
		PointLightFrustum(m_absolutePosition, XMVectorSet(0.0f, -XM_PIDIV2, 0.f, 0.f), lightEntityCached->m_fallOffEnd, viewEntitiesCached.at(1), viewEntityBuffersCached.at(1), depthTestDSVCubeAddressOfCached.at(1)),
		PointLightFrustum(m_absolutePosition, XMVectorSet(-XM_PIDIV2, 0.0f, 0.f, 0.f), lightEntityCached->m_fallOffEnd, viewEntitiesCached.at(2), viewEntityBuffersCached.at(2), depthTestDSVCubeAddressOfCached.at(2)),
		PointLightFrustum(m_absolutePosition, XMVectorSet(XM_PIDIV2, 0.0f, 0.f, 0.f), lightEntityCached->m_fallOffEnd, viewEntitiesCached.at(3), viewEntityBuffersCached.at(3), depthTestDSVCubeAddressOfCached.at(3)),
		PointLightFrustum(m_absolutePosition, XMVectorSet(0.0f, 0.f, 0.f, 0.f), lightEntityCached->m_fallOffEnd, viewEntitiesCached.at(4), viewEntityBuffersCached.at(4), depthTestDSVCubeAddressOfCached.at(4)),
		PointLightFrustum(m_absolutePosition, XMVectorSet(0.0f, XM_PI, 0.f, 0.f), lightEntityCached->m_fallOffEnd, viewEntitiesCached.at(5), viewEntityBuffersCached.at(5), depthTestDSVCubeAddressOfCached.at(5))
	}
{
}

void PointLightComponent::UpdatePointLightFrustums()
{
	for (size_t idx = 0; idx < 6; ++idx)
	{
		m_pointLightFrustums[idx].UpdateViewEntity();
	}
	SetModifiedOption(GetComponentUpdateOption(EPointLightComponentUpdateOption::VIEW_ENTITY));
}

void PointLightComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}

void PointLightComponent::GenerateShadowMap(
	ID3D11DeviceContext* deviceContext, 
	ComponentPSOManager* componentPsoManager, 
	const vector<AComponent*>& components
)
{
	for (size_t idx = 0; idx < 6; ++idx)
	{
		PointLightFrustum& pointLightFrustum = m_pointLightFrustums[idx];
		StructuredBuffer* viewEntityBufferAddress = pointLightFrustum.GetViewEntityBufferAddress();
		vector<AComponent*> renderableComponents = RenderControlOption::GetRenderableComponents(m_parentSceneID, &pointLightFrustum, components);
		
		deviceContext->ClearDepthStencilView(*pointLightFrustum.GetDepthTestDSVCached(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
		DepthTestRenderer depthTestRenderer = DepthTestRenderer(
			deviceContext, componentPsoManager,
			m_componentEntityBuffer.GetBuffer(),
			viewEntityBufferAddress->GetSRV(),
			&m_viewport,
			*pointLightFrustum.GetDepthTestDSVCached()
		);

		for (AComponent* renderableComponent : renderableComponents)
		{
			renderableComponent->Accept(&depthTestRenderer);
		}
	}

}