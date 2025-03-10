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
	: AComponent(componentName, componentID, localPosition, localAngle, XMFLOAT3(1.f, 1.f, 1.f), lightIdx),
	m_lightEntityCached(lightEntityCached), m_lightEntityCachedBuffer(lightEntityCachedBuffer)
{
	UpdateLightEntity(lightPower, fallOffStart, fallOffEnd, spotPower);
}

void LightComponent::UpdateLightEntity(const float& lightPower, const float& fallOffStart, const float& fallOffEnd, const float& spotPower)
{
	m_lightEntityCached->m_lightPower = lightPower;
	m_lightEntityCached->m_fallOffStart = fallOffStart;
	m_lightEntityCached->m_fallOffEnd = fallOffEnd;
	m_lightEntityCached->m_spotPower = spotPower;

	m_lightEntityCachedBuffer->GetBufferChangedFlag().SetFlag(true);
	m_isViewEntityChanged.SetFlag(true);
}
