#include "AComponent.h"

AComponent::AComponent()
{
}

void AComponent::AddChildComponent(AComponent* component)
{
	component->m_parentComponent = this;
	m_childComponents.emplace_back(component);
}
