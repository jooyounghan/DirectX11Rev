#include "AComponent.h"

using namespace DirectX;

AComponent::AComponent(
	const uint32_t& componentID, 
	const XMFLOAT3& position, 
	const XMFLOAT3& rotation, 
	const XMFLOAT3& scale
)
	: ComponentEntity(componentID, position, rotation, scale)
{}

void AComponent::AddChildComponent(AComponent* component)
{
	component->m_parentComponent = this;
	m_childComponents.emplace_back(component);
}
