#include "AComponent.h"
#include "DynamicBuffer.h"
#include "ConstantBuffer.h"

using namespace std;
using namespace DirectX;

SComponent::SComponent(const uint32_t& componentID)
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
	m_localScale(XMVectorSet(localScale.x, localScale.y, localScale.z, 0.f)),
	m_transformationBuffer(new DynamicBuffer(sizeof(STransformation), 1, &m_transformation)),
	m_transformation(),
	m_componentConstant(componentID),
	m_componentBuffer(new DynamicBuffer(sizeof(SComponent), 1, &m_componentConstant)),
	m_componentName(componentName)
{
	m_absolutePosition = XMLoadFloat3(&localPosition);
	m_absoluteAngle = XMLoadFloat3(&localAngle);
	m_absoluteScale = XMLoadFloat3(&localScale);
}

AComponent::~AComponent()
{
	delete m_transformationBuffer;
	delete m_componentBuffer;
}

void AComponent::SetIsModified(const bool& isModified)
{
	m_isModified.store(isModified);
	for (auto& childComponent : m_childComponents)
	{
		childComponent->SetIsModified(isModified);
	}
};

bool AComponent::GetDefaultRenderable()
{ 
	return true; 
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

const DirectX::XMVECTOR AComponent::GetLocalRotationQuaternion()
{
	return XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(XMVectorGetX(m_localAngle)),
		XMConvertToRadians(XMVectorGetY(m_localAngle)),
		XMConvertToRadians(XMVectorGetZ(m_localAngle))
	);
}

const XMVECTOR AComponent::GetAbsoluteRotationQuaternion() const
{
	return XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(XMVectorGetX(m_absoluteAngle)),
		XMConvertToRadians(XMVectorGetY(m_absoluteAngle)),
		XMConvertToRadians(XMVectorGetZ(m_absoluteAngle))
	);
}

DirectX::XMMATRIX AComponent::GetLocalTranformation()
{
	return XMMatrixAffineTransformation(
		m_localScale,
		XMQuaternionIdentity(),
		GetLocalRotationQuaternion(),
		m_localPosition
	);
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