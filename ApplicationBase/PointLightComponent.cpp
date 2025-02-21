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
	SViewEntity* viewEntityCached
)
	: m_absolutePositionCached(absolutePositionCached), 
	m_absoluteRadianOffset(absoluteRadianOffset),
	m_fallOffEndCached(fallOffEndCached),
	m_viewEntityCached(viewEntityCached)
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
	const uint32_t& lightIndex,
	SLightEntity* lightEntityCached,
	StructuredBuffer* lightEntityCachedBuffer,
	const array<SViewEntity*, 6>& viewEntityCached,
	StructuredBuffer* viewCubeEntityCachedBuffer
)
	: LightComponent(componentName, componentID, localPosition, XMFLOAT3(0.f, 0.f, 0.f), lightPower, 
		fallOffStart, fallOffEnd, 1.f, lightIndex, lightEntityCached, lightEntityCachedBuffer
	), m_viewport{ 0.f, 0.f, static_cast<FLOAT>(GDefaultShadowMapWidth), static_cast<FLOAT>(GDefaultShadowMapHeight), 0.f, 1.f },
	m_pointLightFrustums{
		PointLightFrustum(m_absolutePosition, XMVectorSet(0.0f, XM_PIDIV2, 0.f, 0.f), lightEntityCached->m_fallOffEnd, viewEntityCached[0]),
		PointLightFrustum(m_absolutePosition, XMVectorSet(0.0f, -XM_PIDIV2, 0.f, 0.f), lightEntityCached->m_fallOffEnd, viewEntityCached[1]),
		PointLightFrustum(m_absolutePosition, XMVectorSet(-XM_PIDIV2, 0.0f, 0.f, 0.f), lightEntityCached->m_fallOffEnd, viewEntityCached[2]),
		PointLightFrustum(m_absolutePosition, XMVectorSet(XM_PIDIV2, 0.0f, 0.f, 0.f), lightEntityCached->m_fallOffEnd, viewEntityCached[3]),
		PointLightFrustum(m_absolutePosition, XMVectorSet(0.0f, 0.f, 0.f, 0.f), lightEntityCached->m_fallOffEnd, viewEntityCached[4]),
		PointLightFrustum(m_absolutePosition, XMVectorSet(0.0f, XM_PI, 0.f, 0.f), lightEntityCached->m_fallOffEnd, viewEntityCached[5]),
	},
	m_viewCubeEntityCachedBuffer(viewCubeEntityCachedBuffer),
	m_deptTestViewCube(GDefaultShadowMapWidth, GDefaultShadowMapHeight, 6, 1, NULL, D3D11_RESOURCE_MISC_TEXTURECUBE, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_TYPELESS, D3D11_BIND_DEPTH_STENCIL)
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
		vector<AComponent*> renderableComponents = RenderControlOption::GetRenderableComponents(m_parentSceneID, &pointLightFrustum, components);

		deviceContext->ClearDepthStencilView(pointLightFrustum.GetDepthTestDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
		DepthTestRenderer depthTestRenderer = DepthTestRenderer(
			deviceContext, componentPsoManager,
			m_componentEntityBuffer.GetBuffer(),
			m_viewCubeEntityCachedBuffer->GetSRV(),
			m_lightEntityCachedBuffer->GetSRV(),
			&m_viewport,
			pointLightFrustum.GetDepthTestDSV()
		);

		for (AComponent* renderableComponent : renderableComponents)
		{
			renderableComponent->Accept(&depthTestRenderer);
		}
	}

}