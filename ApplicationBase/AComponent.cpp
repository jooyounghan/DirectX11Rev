#include "AComponent.h"

using namespace std;
using namespace DirectX;

SComponentEntity::SComponentEntity(
	const uint32_t& componentID,
	const uint32_t& componentVariable1,
	const uint32_t& componentVariable2,
	const uint32_t& componentVariable3
	
	)
	: m_componentID(componentID),
	m_componentVariable1(componentVariable1),
	m_componentVariable2(componentVariable2),
	m_componentVariable3(componentVariable3)
{
}


AComponent::AComponent(
	const string& componentName,
	const uint32_t& componentID, 
	const XMFLOAT3& localPosition,
	const XMFLOAT3& localAngle,
	const XMFLOAT3& localScale,
	const uint32_t& componentVariable1,
	const uint32_t& componentVariable2,
	const uint32_t& componentVariable3
)	: 
	m_localPosition(XMVectorSet(localPosition.x, localPosition.y, localPosition.z, 0.f)),
	m_localAngle(XMVectorSet(localAngle.x, localAngle.y, localAngle.z, 0.f)),
	m_scale(XMVectorSet(localScale.x, localScale.y, localScale.z, 0.f)),
	m_transformationEntityBuffer(sizeof(STransformationEntity), 1, &m_transformationEntity),
	m_componentEntity(componentID, componentVariable1, componentVariable2, componentVariable3),
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

	m_isTransformationEntityUpdated.SetFlag(true);
	m_isDBUpdated.SetFlag(true);
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

	m_isTransformationEntityUpdated.SetFlag(true);
	m_isDBUpdated.SetFlag(true);
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

const DirectX::XMMATRIX& AComponent::GetAbsoluteTranformation() const { return m_transformation; }

void AComponent::SetLocalPosition(const DirectX::XMVECTOR& localPosition)
{
	m_localPosition = localPosition;
	m_isTransformationEntityUpdated.SetFlag(true);
	m_isDBUpdated.SetFlag(true);
}

void AComponent::SetLocalAngle(const DirectX::XMVECTOR& localAngle)
{
	m_localAngle = localAngle;
	m_isTransformationEntityUpdated.SetFlag(true);
	m_isDBUpdated.SetFlag(true);
}

void AComponent::SetScale(const DirectX::XMVECTOR& scale)
{
	m_scale = scale;
	m_isTransformationEntityUpdated.SetFlag(true);
	m_isDBUpdated.SetFlag(true);
}

void AComponent::UpdateEntity(ID3D11DeviceContext* deviceContext)
{
	m_absolutePosition = m_parentComponent ? m_parentComponent->GetAbsolutePosition() + m_localPosition : m_localPosition;
	m_absoluteAngle = m_parentComponent ? m_parentComponent->GetAbsoluteAngle() + m_localAngle : m_localAngle;

	const XMMATRIX parentTransformation = m_parentComponent ? m_parentComponent->GetAbsoluteTranformation() : XMMatrixIdentity();
	m_transformation = GetLocalTransformation() * parentTransformation;
	m_transformationEntity.m_invTransformation = XMMatrixInverse(nullptr, m_transformation);
	m_transformationEntity.m_transformation = XMMatrixTranspose(m_transformation);

	m_transformationEntityBuffer.Upload(deviceContext);

	for (AComponent* childComponent : m_childComponents)
	{
		childComponent->UpdateEntity(deviceContext);
	}
}

