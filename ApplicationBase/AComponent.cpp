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

DirectX::XMMATRIX AComponent::GetAbsoluteTransformation()
{
	return m_parentComponent ?
		GetLocalTranformation() * m_parentComponent->GetAbsoluteTransformation() :
		GetLocalTranformation();
}

void AComponent::UpdateAbsoluteEntities(const XMMATRIX* parentAbsTransformation)
{
	const XMMATRIX matrix = parentAbsTransformation ?
		GetLocalTranformation() * (*parentAbsTransformation) :
		GetAbsoluteTransformation();
	
	XMVECTOR absRotationQuaternion;
	
	XMMatrixDecompose(
		&m_absoluteScale, 
		&absRotationQuaternion, 
		&m_absolutePosition, 
		matrix
	);

	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(absRotationQuaternion);

	float& pitch = m_absoluteAngle.m128_f32[0];
	float& yaw = m_absoluteAngle.m128_f32[1];
	float& roll = m_absoluteAngle.m128_f32[2];

	pitch = asinf(-rotationMatrix.r[2].m128_f32[0]);
	if (cosf(pitch) > 1e-6) 
	{
		yaw = atan2f(rotationMatrix.r[2].m128_f32[1], rotationMatrix.r[2].m128_f32[2]);
		roll = atan2f(rotationMatrix.r[1].m128_f32[0], rotationMatrix.r[0].m128_f32[0]);
	}
	else 
	{
		yaw = 0.0f;
		roll = atan2f(-rotationMatrix.r[0].m128_f32[1], rotationMatrix.r[1].m128_f32[1]);
	}

	pitch = XMConvertToDegrees(pitch);
	yaw = XMConvertToDegrees(yaw);
	roll = XMConvertToDegrees(roll);

	for (AComponent* childComponent : m_childComponents)
	{
		childComponent->UpdateAbsoluteEntities(&matrix);
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