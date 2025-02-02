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
}

PointLightComponent::~PointLightComponent()
{
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

