#include "LightEntity.h"

using namespace std;

unordered_map<uint32_t, LightEntity*> LightEntity::Lights;

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
