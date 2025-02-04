#include "PointLightComponent.h"

#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "DSVOption.h"

#include "DepthTestRenderer.h"
#include "RenderControlOption.h"

#include "DynamicBuffer.h"

using namespace std;
using namespace DirectX;

PointLightFrustumPart::PointLightFrustumPart(
	const XMFLOAT3& rotationRadian, 
	D3D11_VIEWPORT& viewport,
	XMVECTOR& absolutePosition,
	float& fallOffEnd
)
	: m_rotationRadian(rotationRadian), m_viewport(viewport), m_absolutePosition(absolutePosition), m_fallOffEnd(fallOffEnd)
{
}

DirectX::XMMATRIX PointLightFrustumPart::GetViewMatrix() const
{
	const XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(
		m_rotationRadian.x,
		m_rotationRadian.y,
		m_rotationRadian.z
	);

	XMVECTOR currentForward = XMVector3Rotate(GDefaultForward, quaternion);
	XMVECTOR currentUp = XMVector3Rotate(GDefaultUp, quaternion);
	return XMMatrixLookToLH(m_absolutePosition, currentForward, currentUp);
}

DirectX::XMMATRIX PointLightFrustumPart::GetProjectionMatrix() const
{
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f), m_viewport.Width / m_viewport.Height, 0.01f, m_fallOffEnd);
}

void PointLightFrustumPart::UpdateViewEntity()
{
	const XMMATRIX viewMatrix = GetViewMatrix();
	const XMMATRIX projectionMatrix = GetProjectionMatrix();

	UpdateViewEntityImpl(viewMatrix, projectionMatrix, m_absolutePosition);

	BoundingFrustum::CreateFromMatrix(*this, projectionMatrix);
	this->Transform(*this, XMMatrixInverse(nullptr, viewMatrix));
}

void PointLightFrustumPart::OnCollide(ICollisionAcceptor*)
{
}


PointLightComponent::PointLightComponent(
	const std::string& componentName,
	const uint32_t& componentID,
	const XMFLOAT3& position,
	const XMFLOAT3& angle,
	const XMFLOAT3& scale,
	const uint32_t& width,
	const uint32_t& height
)
	: AComponent(componentName, componentID, position, angle, scale),
	m_pointLightFrustumPart{
		PointLightFrustumPart(XMFLOAT3(0.0f, XM_PIDIV2, 0.0f), m_viewport, m_absolutePosition, m_lightEntity.m_fallOffEnd),
		PointLightFrustumPart(XMFLOAT3(0.0f, -XM_PIDIV2, 0.0f), m_viewport, m_absolutePosition, m_lightEntity.m_fallOffEnd),
		PointLightFrustumPart(XMFLOAT3(-XM_PIDIV2, 0.0f, 0.0f), m_viewport, m_absolutePosition, m_lightEntity.m_fallOffEnd),
		PointLightFrustumPart(XMFLOAT3(XM_PIDIV2, 0.0f, 0.0f), m_viewport, m_absolutePosition, m_lightEntity.m_fallOffEnd),
		PointLightFrustumPart(XMFLOAT3(0.0f, 0.f, 0.0f), m_viewport, m_absolutePosition, m_lightEntity.m_fallOffEnd),
		PointLightFrustumPart(XMFLOAT3(0.0f, XM_PI, 0.0f), m_viewport, m_absolutePosition, m_lightEntity.m_fallOffEnd)
	}
{
	m_viewport.TopLeftX = 0.f;
	m_viewport.TopLeftY = 0.f;
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;
}

PointLightComponent::~PointLightComponent()
{
	if (m_deptTestViewCube) delete m_deptTestViewCube;
}

void PointLightComponent::SetDepthTestViewCube(Texture2DInstance<SRVOption>* depthTestViewCube)
{
	if (m_deptTestViewCube) delete m_deptTestViewCube;
	m_deptTestViewCube = depthTestViewCube;
}

void PointLightComponent::UpdatePointLightParts()
{
	for (size_t idx = 0; idx < 6; ++idx)
	{
		m_pointLightFrustumPart[idx].UpdateViewEntity();
	}
}

void PointLightComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}

void PointLightComponent::GenerateShadowMap(
	ID3D11DeviceContext* const* deviceContextAddress, 
	ComponentPSOManager* componentPsoManager, 
	const vector<AComponent*>& components
)
{
	for (size_t idx = 0; idx < 6; ++idx)
	{
		PointLightFrustumPart& pointLightFrustumPart = m_pointLightFrustumPart[idx];
		vector<AComponent*> renderableComponents = RenderControlOption::GetRenderableComponents(&pointLightFrustumPart, components);

		(*deviceContextAddress)->ClearDepthStencilView(pointLightFrustumPart.GetDepthTestDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
		DepthTestRenderer depthTestRenderer = DepthTestRenderer(
			deviceContextAddress, componentPsoManager,
			pointLightFrustumPart.GetViewProjMatrixBuffer()->GetBuffer(),
			&m_viewport,
			pointLightFrustumPart.GetDepthTestDSV()
		);

		for (AComponent* renderableComponent : renderableComponents)
		{
			renderableComponent->Accept(&depthTestRenderer);
		}
	}

}