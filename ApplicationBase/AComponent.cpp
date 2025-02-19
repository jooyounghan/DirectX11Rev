#include "AComponent.h"

using namespace std;
using namespace DirectX;

SComponentEntity::SComponentEntity(const uint32_t& componentID)
	: m_componentID(componentID), dummy{ 0.f, 0.f, 0.f }
{
}


AComponent::AComponent(
	const string& componentName,
	const uint32_t& componentID, 
	const XMFLOAT3& localPosition,
	const XMFLOAT3& localAngle,
	const XMFLOAT3& localScale
)	: 
	m_localPosition(XMVectorSet(localPosition.x, localPosition.y, localPosition.z, 0.f)),
	m_localAngle(XMVectorSet(localAngle.x, localAngle.y, localAngle.z, 0.f)),
	m_scale(XMVectorSet(localScale.x, localScale.y, localScale.z, 0.f)),
	m_transformationEntityBuffer(sizeof(STransformationEntity), 1, &m_transformationEntity),
	m_componentEntity(componentID),
	m_componentEntityBuffer(sizeof(SComponentEntity), 1, &m_componentEntity),
	m_componentName(componentName)
{
	m_absolutePosition = XMLoadFloat3(&localPosition);
	m_absoluteAngle = XMLoadFloat3(&localAngle);
}

bool AComponent::GetDefaultRenderable() const
{ 
	return true; 
}

void AComponent::AttachChildComponent(AComponent* component)
{
	component->m_parentComponent = this;
	m_childComponents.emplace_back(component);

	component->UpdateEntity();
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
	
	UpdateEntity();
}

DirectX::XMVECTOR AComponent::GetLocalRotationQuaternion() const
{
	return XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(XMVectorGetX(m_localAngle)),
		XMConvertToRadians(XMVectorGetY(m_localAngle)),
		XMConvertToRadians(XMVectorGetZ(m_localAngle))
	);
}

XMVECTOR AComponent::GetAbsoluteRotationQuaternion() const
{
	return XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(XMVectorGetX(m_absoluteAngle)),
		XMConvertToRadians(XMVectorGetY(m_absoluteAngle)),
		XMConvertToRadians(XMVectorGetZ(m_absoluteAngle))
	);
}

DirectX::XMMATRIX AComponent::GetLocalTransformation() const
{
	return XMMatrixAffineTransformation(
		m_scale,
		XMQuaternionIdentity(),
		GetLocalRotationQuaternion(),
		m_localPosition
	);
}

void AComponent::UpdateEntity()
{
	m_absolutePosition = m_parentComponent ? m_parentComponent->GetAbsolutePosition() + m_localPosition : m_localPosition;
	m_absoluteAngle = m_parentComponent ? m_parentComponent->GetAbsoluteAngle() + m_localAngle : m_localAngle;

	const XMMATRIX parentTransformation = m_parentComponent ? m_parentComponent->GetAbsoluteTranformation() : XMMatrixIdentity();
	m_transformation = parentTransformation * GetLocalTransformation();
	m_transformationEntity.m_invTransformation = XMMatrixInverse(nullptr, m_transformation);
	m_transformationEntity.m_transformation = XMMatrixTranspose(m_transformation);

	for (AComponent* childComponent : m_childComponents)
	{
		childComponent->UpdateEntity();
	}

	m_isModified.store(true);
}