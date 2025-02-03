#include "PointLightComponent.h"

#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "DSVOption.h"

#include "DynamicBuffer.h"

using namespace std;
using namespace DirectX;

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
	m_viewPorjBuffers(new DynamicBuffer(sizeof(SViewEntity) * 6, 1, m_viewEntities))
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

XMMATRIX PointLightComponent::GetViewMatrix(const size_t& idx) const
{
	static const XMFLOAT3 rotationAngles[6] =
	{
		{	XM_PIDIV2,   0.0f,			0.0f		},
		{	-XM_PIDIV2,  0.0f,			0.0f		},
		{	0.0f,        XM_PIDIV2,		0.0f		},
		{	0.0f,       -XM_PIDIV2,		0.0f		},
		{	0.0f,        0.0f,			XM_PIDIV2	},
		{	0.0f,        0.0f,			-XM_PIDIV2	}
	};
	
	if (idx < 6)
	{
		const XMFLOAT3& rotationAngle = rotationAngles[idx];
		const XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(
			rotationAngle.x,
			rotationAngle.y,
			rotationAngle.z
		);

		XMVECTOR currentForward = XMVector3Rotate(GDefaultForward, quaternion);
		XMVECTOR currentUp = XMVector3Rotate(GDefaultUp, quaternion);
		return XMMatrixLookToLH(m_absolutePosition, currentForward, currentUp);
	}
	else
	{
		return XMMatrixIdentity();
	}
}

XMMATRIX PointLightComponent::GetProjectionMatrix() const
{
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f), m_viewport.Width / m_viewport.Height, 0.01f, m_lightEntity.m_fallOffEnd);
}

DynamicBuffer* PointLightComponent::GetViewProjMatrixBuffer() const { return m_viewPorjBuffers; }

void PointLightComponent::UpdateViewEntity()
{
	const XMMATRIX projectionMatrix = GetProjectionMatrix();
	for (size_t idx = 0; idx < 6; ++idx)
	{
		const XMMATRIX viewMatrix = GetViewMatrix(idx);

		m_viewEntities[idx].m_viewProj = viewMatrix * projectionMatrix;
		m_viewEntities[idx].m_invViewProj = XMMatrixInverse(nullptr, m_viewEntities[idx].m_viewProj);
		m_viewEntities[idx].m_viewProj = XMMatrixTranspose(m_viewEntities[idx].m_viewProj);
		XMStoreFloat3(&m_viewEntities[idx].m_viewPosition, m_absolutePosition);
	}

	XMStoreFloat3(&Center, m_absolutePosition);
	Radius = m_lightEntity.m_fallOffEnd;
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

}

void PointLightComponent::OnCollide(ICollisionAcceptor*)
{
}

