#include "SpotLightComponent.h"
#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "DSVOption.h"

#include "Scene.h"
#include "DepthTestRenderer.h"

#include "RenderControlOption.h"
#include "DepthTestRenderer.h"

#include "DynamicBuffer.h"

using namespace std;
using namespace DirectX;

SpotLightComponent::SpotLightComponent(
	const string& componentName, 
	const uint32_t& componentID, 
	const XMFLOAT3& position, 
	const XMFLOAT3& angle, 
	const XMFLOAT3& scale, 
	const uint32_t& width, const uint32_t& height,
	const float& nearZ, const float& farZ, 
	const float& fovAngle
)
	: AViewComponent(componentName, componentID, position, angle, scale, width, height, nearZ, farZ, fovAngle)
{
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
	RenderControlOption::RenderBVH.Traverse(this);

	(*deviceContextAddress)->ClearDepthStencilView(m_depthTestView->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	DepthTestRenderer depthTestRenderer = DepthTestRenderer(
		deviceContextAddress, componentPsoManager,
		m_viewProjBuffer->GetBuffer(),
		this,
		m_depthTestView
	);

	for (AComponent* component : components)
	{
		DepthTestImpl(component, depthTestRenderer);
	}
}

void SpotLightComponent::DepthTestImpl(AComponent* component, DepthTestRenderer& depthTestRenderer)
{
	if (component->IsRenderable())
	{
		component->Accept(&depthTestRenderer);
		const vector<AComponent*>& childComponents = component->GetChildComponents();
		for (AComponent* childComponent : childComponents)
		{
			DepthTestImpl(childComponent, depthTestRenderer);
		}
	}
}
