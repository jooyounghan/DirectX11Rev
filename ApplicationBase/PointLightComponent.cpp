#include "PointLightComponent.h"

#include "RenderControlOption.h"
#include "StructuredBuffer.h"
#include "PointLightDepthTestRenderer.h"

using namespace std;
using namespace DirectX;

PointLightFrustum::PointLightFrustum(
	const XMVECTOR& absolutePositionCached,
	const XMVECTOR& absoluteRadianOffset,
	const float& fallOffEndCached,
	ID3D11DepthStencilView** depthTestDSVAddressOfCached
)
	: m_absolutePositionCached(absolutePositionCached), 
	m_absoluteRadianOffset(absoluteRadianOffset),
	m_fallOffEndCached(fallOffEndCached),
	m_depthTestDSVAddressOfCached(depthTestDSVAddressOfCached),
	m_viewEntityBuffer(sizeof(SViewEntity), 1, &m_viewEntity)
{
}

void PointLightFrustum::UpdateViewEntity(ID3D11DeviceContext* deviceContext)
{	
	const XMVECTOR quaternion = XMQuaternionRotationRollPitchYawFromVector(m_absoluteRadianOffset);
	XMVECTOR currentForward = XMVector3Rotate(GDefaultForward, quaternion);
	XMVECTOR currentUp = XMVector3Rotate(GDefaultUp, quaternion);

	m_viewMatrix = XMMatrixLookToLH(m_absolutePositionCached, currentForward, currentUp);
	m_projMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f), 1.f, GDefaultNearZ, m_fallOffEndCached);

	m_viewEntity.m_viewProj = m_viewMatrix * m_projMatrix;
	m_viewEntity.m_invViewProj = XMMatrixInverse(nullptr, m_viewEntity.m_viewProj);
	m_viewEntity.m_viewProj = XMMatrixTranspose(m_viewEntity.m_viewProj);
	XMStoreFloat3(&m_viewEntity.m_viewPosition, m_absolutePositionCached);

	m_viewEntityBuffer.Upload(deviceContext);

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
	StructuredBuffer* lightEntitiesBufferCached,
	XMVECTOR* lightPositionCached,
	StructuredBuffer* lightPositionsBufferCached,
	const std::array<ID3D11DepthStencilView**, 6>& depthTestDSVCubeAddressOfCached
)
	: LightComponent(componentName, componentID, localPosition, XMFLOAT3(0.f, 0.f, 0.f), lightPower, 
		fallOffStart, fallOffEnd, 1.f, lightIdx, lightEntityCached, lightEntitiesBufferCached
	), m_viewport{ 0.f, 0.f, static_cast<FLOAT>(GDefaultShadowMapWidth), static_cast<FLOAT>(GDefaultShadowMapHeight), 0.f, 1.f },
	m_lightPositionCached(lightPositionCached),
	m_lightPositionsBufferCached(lightPositionsBufferCached),
	m_pointLightFrustums{
		PointLightFrustum(m_absolutePosition, XMVectorSet(0.0f, XM_PIDIV2, 0.f, 0.f), lightEntityCached->m_fallOffEnd, depthTestDSVCubeAddressOfCached.at(0)),
		PointLightFrustum(m_absolutePosition, XMVectorSet(0.0f, -XM_PIDIV2, 0.f, 0.f), lightEntityCached->m_fallOffEnd, depthTestDSVCubeAddressOfCached.at(1)),
		PointLightFrustum(m_absolutePosition, XMVectorSet(-XM_PIDIV2, 0.0f, 0.f, 0.f), lightEntityCached->m_fallOffEnd, depthTestDSVCubeAddressOfCached.at(2)),
		PointLightFrustum(m_absolutePosition, XMVectorSet(XM_PIDIV2, 0.0f, 0.f, 0.f), lightEntityCached->m_fallOffEnd, depthTestDSVCubeAddressOfCached.at(3)),
		PointLightFrustum(m_absolutePosition, XMVectorSet(0.0f, 0.f, 0.f, 0.f), lightEntityCached->m_fallOffEnd, depthTestDSVCubeAddressOfCached.at(4)),
		PointLightFrustum(m_absolutePosition, XMVectorSet(0.0f, XM_PI, 0.f, 0.f), lightEntityCached->m_fallOffEnd, depthTestDSVCubeAddressOfCached.at(5))
	}
{
}

void PointLightComponent::UpdatePointLightFrustums(ID3D11DeviceContext* deviceContext)
{
	for (size_t idx = 0; idx < 6; ++idx)
	{
		m_pointLightFrustums[idx].UpdateViewEntity(deviceContext);
	}
}

void PointLightComponent::UpdateEntity(ID3D11DeviceContext* deviceContext)
{
	LightComponent::UpdateEntity(deviceContext);

	memcpy(m_lightPositionCached, &m_absolutePosition, sizeof(XMVECTOR));
	m_lightPositionsBufferCached->GetBufferChangedFlag().SetFlag(true);

	UpdatePointLightFrustums(deviceContext);
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
		DynamicBuffer& viewEntityBuffer = pointLightFrustum.GetViewEntityBuffer();
		vector<AComponent*> renderableComponents = RenderControlOption::GetRenderableComponents(m_parentSceneID, &pointLightFrustum, components);
		
		deviceContext->ClearDepthStencilView(*pointLightFrustum.GetDepthTestDSVCached(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
		PointLightDepthTestRenderer depthTestRenderer = PointLightDepthTestRenderer(
			deviceContext, componentPsoManager,
			viewEntityBuffer.GetBuffer(),
			&m_viewport,
			*pointLightFrustum.GetDepthTestDSVCached()
		);

		for (AComponent* renderableComponent : renderableComponents)
		{
			renderableComponent->Accept(&depthTestRenderer);
		}
	}

}