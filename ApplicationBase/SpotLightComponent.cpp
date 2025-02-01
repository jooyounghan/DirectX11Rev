#include "SpotLightComponent.h"
#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "DSVOption.h"

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
	Lights.emplace(componentID, this);
}

SpotLightComponent::~SpotLightComponent()
{
	Lights.erase(m_componentConstant.m_componentID);
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
