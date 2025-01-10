#include "AComponent.h"

using namespace std;
using namespace DirectX;

AComponent::AComponent(
	const string& componentName,
	const uint32_t& componentID, 
	const XMFLOAT3& relativePosition,
	const XMFLOAT3& relativeAngle,
	const XMFLOAT3& relativeScale
)
	: 
	
	ComponentEntity(componentID), 
	m_relativePosition(XMVectorSet(relativePosition.x, relativePosition.y, relativePosition.z, 0.f)),
	m_relativeAngle(XMVectorSet(relativeAngle.x, relativeAngle.y, relativeAngle.z, 0.f)),
	m_relativeScale(XMVectorSet(relativeScale.x, relativeScale.y, relativeScale.z, 0.f)),
	m_componentName(componentName)
{}

void AComponent::AttachChildComponent(AComponent* component)
{
	component->m_parentComponent = this;
	m_childComponents.emplace_back(component);
}

void AComponent::DetachChildComponent(AComponent* component)
{
	if (component->GetParentComponent() == this)
	{
		m_childComponents.erase(
			std::remove(m_childComponents.begin(), m_childComponents.end(), component),
			m_childComponents.end()
		);
	}
}

void AComponent::RemoveFromParent()
{
	if (m_parentComponent != nullptr)
	{
		m_parentComponent->DetachChildComponent(this);
	}
	m_parentComponent = nullptr;
}

XMVECTOR AComponent::GetAbsolutePosition() { return XMVectorAdd(m_relativePosition, m_parentComponent == nullptr ? XMVectorZero() : m_parentComponent->GetAbsolutePosition()); }
XMVECTOR AComponent::GetAbsoluteAngle() { return XMVectorAddAngles(m_relativeAngle, m_parentComponent == nullptr ? XMVectorZero() : m_parentComponent->GetAbsoluteAngle()); }
XMVECTOR AComponent::GetAbsoluteScale() { return XMVectorAdd(m_relativeScale, m_parentComponent == nullptr ? XMVectorZero() : m_parentComponent->GetAbsoluteScale()); }

void AComponent::UpdateAbsoluteEntities()
{
	m_absolutePosition = GetAbsolutePosition();
	m_absoluteAngle = GetAbsoluteAngle();
	m_absoluteScale = GetAbsoluteScale();

	for (AComponent* childComponent : m_childComponents)
	{
		childComponent->UpdateAbsoluteEntities();
	}
}