#include "AComponent.h"

using namespace DirectX;

AComponent::AComponent(const uint64_t& componentID, XMFLOAT3 position) 
	: ComponentEntity(componentID, position) 
{}

void AComponent::AddChildComponent(AComponent* component)
{
	component->m_parentComponent = this;
	m_childComponents.emplace_back(component);
}
