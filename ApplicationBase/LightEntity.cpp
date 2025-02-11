#include "LightEntity.h"
#include "DynamicBuffer.h"

using namespace std;

LightEntity::LightEntity()
	: m_lightEntityBuffer(new DynamicBuffer(sizeof(SLightEntity), 1, &m_lightEntity))
{
}

void LightEntity::SetLigthEntity(
	const float& lightPower, 
	const float& fallOffStart, 
	const float& fallOffEnd, 
	const float& spotPower
)
{
	m_lightEntity.m_lightPower = lightPower;
	m_lightEntity.m_fallOffStart = fallOffStart;
	m_lightEntity.m_fallOffEnd = fallOffEnd;
	m_lightEntity.m_spotPower = spotPower;
}
