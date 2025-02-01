#include "PointLightComponent.h"

using namespace std;
using namespace DirectX;

PointLightComponent::PointLightComponent(
	const std::string& componentName, 
	const uint32_t& componentID, 
	const DirectX::XMFLOAT3& position, 
	const DirectX::XMFLOAT3& angle, 
	const DirectX::XMFLOAT3& scale
)
	: AComponent(componentName, componentID, position, angle, scale)
{
	Lights.emplace(componentID, this);
}

PointLightComponent::~PointLightComponent()
{
	Lights.erase(m_componentConstant.m_componentID);
}

void PointLightComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}
