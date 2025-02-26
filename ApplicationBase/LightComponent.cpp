#include "LightComponent.h"
#include "StructuredBuffer.h"

using namespace std;
using namespace DirectX;

LightComponent::LightComponent(
	const string& componentName,
	const uint32_t& componentID,
	const XMFLOAT3& localPosition,
	const XMFLOAT3& localAngle,
	const float& lightPower,
	const float& fallOffStart,
	const float& fallOffEnd,
	const float& spotPower,
	const uint32_t& lightIdx,
	SLightEntity* lightEntityCached,
	StructuredBuffer* lightEntityCachedBuffer
)
	: AComponent(componentName, componentID, localPosition, localAngle, XMFLOAT3(1.f, 1.f, 1.f)),
	m_lightEntityCached(lightEntityCached), m_lightEntityCachedBuffer(lightEntityCachedBuffer)
{
	m_componentEntity.m_componentVariable1 = lightIdx;
	UpdateLightEntity(lightPower, fallOffStart, fallOffEnd, spotPower);
}

void LightComponent::UpdateLightEntity()
{
	if (m_lightEntityCached)
	{
		SetModifiedOption(GetComponentUpdateOption(ELightComponentUpdateOption::LIGHT_ENTITY));
	}
}


void LightComponent::UpdateLightEntity(const float& lightPower, const float& fallOffStart, const float& fallOffEnd, const float& spotPower)
{
	if (m_lightEntityCached)
	{
		m_lightEntityCached->m_lightPower = lightPower;
		m_lightEntityCached->m_fallOffStart = fallOffStart;
		m_lightEntityCached->m_fallOffEnd = fallOffEnd;
		m_lightEntityCached->m_spotPower = spotPower;
		SetModifiedOption(GetComponentUpdateOption(ELightComponentUpdateOption::LIGHT_ENTITY));
	}
}
