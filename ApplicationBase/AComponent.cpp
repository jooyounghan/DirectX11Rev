#include "AComponent.h"

#include <DirectXMath.h>

using namespace std;
using namespace DirectX;

AComponent::AComponent(
	const string& componentName,
	const uint32_t& componentID, 
	const XMFLOAT3& localPosition,
	const XMFLOAT3& localAngle,
	const XMFLOAT3& localScale
)	: 
	ComponentEntity(componentID, localPosition, localAngle, localScale), 
	m_componentName(componentName)
{
	m_absolutePosition = XMLoadFloat3(&localPosition);
	m_absoluteAngle = XMLoadFloat3(&localAngle);
	m_absoluteScale = XMLoadFloat3(&localScale);
}

void AComponent::AttachChildComponent(AComponent* component)
{
	component->m_parentComponent = this;
	m_childComponents.emplace_back(component);
	component->UpdateAbsoluteEntities();
}

void AComponent::RemoveFromParent()
{
	if (m_parentComponent != nullptr)
	{
		vector<AComponent*> parentsChildComponents = m_parentComponent->m_childComponents;
		parentsChildComponents.erase(
			std::remove(parentsChildComponents.begin(), parentsChildComponents.end(), this),
			parentsChildComponents.end()
		);
	}
	m_parentComponent = nullptr;
	UpdateAbsoluteEntities();
}

const XMVECTOR AComponent::GetAbsoluteRotationQuaternionV() const
{
	return XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(XMVectorGetX(m_absoluteAngle)),
		XMConvertToRadians(XMVectorGetY(m_absoluteAngle)),
		XMConvertToRadians(XMVectorGetZ(m_absoluteAngle))
	);
}

const XMFLOAT4 AComponent::GetAbsoluteRotationQuaternion() const
{ 
	const XMVECTOR rotationQuaternionV = GetAbsoluteRotationQuaternionV();
	XMFLOAT4 rotationQuaternion;
	XMStoreFloat4(&rotationQuaternion, rotationQuaternionV);
	return rotationQuaternion;
}

XMMATRIX AComponent::GetAbsoluteTransformation()
{
	return m_parentComponent ?
		GetLocalTranformation() * m_parentComponent->GetAbsoluteTransformation() :
		GetLocalTranformation();
}

void AComponent::UpdateAbsoluteEntities()
{
	m_absolutePosition = m_parentComponent ? m_parentComponent->GetAbsolutePosition() + m_localPosition : m_localPosition;
	m_absoluteAngle = m_parentComponent ? m_parentComponent->GetAbsoluteAngle() + m_localAngle : m_localAngle;
	m_absoluteScale = m_parentComponent ? m_parentComponent->GetAbsoluteScale() * m_localScale: m_localScale;

	for (AComponent* childComponent : m_childComponents)
	{
		childComponent->UpdateAbsoluteEntities();
	}
}

void AComponent::UpdateComponentTransformation()
{
	m_transformation.m_transformation = GetAbsoluteTransformation();
	m_transformation.m_invTransformation = XMMatrixInverse(nullptr, m_transformation.m_transformation);
	m_transformation.m_transformation = XMMatrixTranspose(m_transformation.m_transformation);
}

void AComponent::SetIsModified(const bool& isModified)
{
	m_isModified.store(isModified);
	for (auto& childComponent : m_childComponents)
	{
		childComponent->SetIsModified(isModified);
	}
};