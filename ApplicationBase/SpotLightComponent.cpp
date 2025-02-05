#include "SpotLightComponent.h"
#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "DSVOption.h"

#include "DepthTestRenderer.h"
#include "RenderControlOption.h"

#include "DynamicBuffer.h"

using namespace std;
using namespace DirectX;

SpotLightComponent::SpotLightComponent(
	const string& componentName, 
	const uint32_t& componentID, 
	const XMFLOAT3& position, 
	const XMFLOAT3& angle, 
	const XMFLOAT3& scale, 
	const float& fovAngle,
	const float& farZ
)
	: AViewComponent(
		componentName, componentID, 
		position, angle, scale, 
		GDefaultShadowMapWidth, GDefaultShadowMapHeight, fovAngle
	)
{
	m_lightEntity.m_fallOffEnd = farZ;
}

SpotLightComponent::~SpotLightComponent()
{
	if (m_depthTestView) delete m_depthTestView;
}

void SpotLightComponent::SetDepthTestView(Texture2DInstance<SRVOption, DSVOption>* depthTestView)
{
	if (m_depthTestView) delete m_depthTestView;
	m_depthTestView = depthTestView;
}

XMMATRIX SpotLightComponent::GetProjectionMatrix() const
{
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fovAngle), Width / Height, 0.01f, m_lightEntity.m_fallOffEnd);
}

void SpotLightComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}

void SpotLightComponent::GenerateShadowMap(
	ID3D11DeviceContext* const* deviceContextAddress, 
	ComponentPSOManager* componentPsoManager, 
	const vector<AComponent*>& components
)
{
	vector<AComponent*> renderableComponents = RenderControlOption::GetRenderableComponents(m_parentSceneID, this, components);

	(*deviceContextAddress)->ClearDepthStencilView(m_depthTestView->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	DepthTestRenderer depthTestRenderer = DepthTestRenderer(
		deviceContextAddress, componentPsoManager,
		m_viewProjBuffer->GetBuffer(),
		this,
		m_depthTestView->GetDSV()
	);

	for (AComponent* renderableComponent : renderableComponents)
	{
		renderableComponent->Accept(&depthTestRenderer);
	}
}
