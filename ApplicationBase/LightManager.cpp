#include "LightManager.h"
#include "SpotLightComponent.h"
#include "PointLightComponent.h"

using namespace std;
using namespace DirectX;

LightManager::LightManager()
	: m_spotLightEntityBuffer(sizeof(SLightEntity), static_cast<UINT>(m_spotLightEntities.size()), m_spotLightEntities.data()),
	m_spotLightViewEntityBuffer(sizeof(SViewEntity), static_cast<UINT>(m_spotLightViewEntities.size()), m_spotLightViewEntities.data()),
	m_pointLightEntityBuffer(sizeof(SLightEntity), static_cast<UINT>(m_pointLightEntities.size()), m_pointLightEntities.data()),
	m_pointLightViewCubeEntityBuffer(sizeof(SViewEntity) * 6, static_cast<UINT>(m_pointLightViewCubeEntities.size()), m_pointLightViewCubeEntities.data())
{
}

SpotLightComponent* LightManager::CreateSpotLight(
	const string& componentName, 
	const uint32_t& componentID, 
	const XMFLOAT3& localPosition, 
	const XMFLOAT3& localAngle, 
	const float& lightPower, 
	const float& fallOffStart, 
	const float& fallOffEnd, 
	const float& spotPower, 
	const float& fovAngle
)
{
	SLightEntity& lightEntity = m_spotLightEntities[m_lastSpotLightIndex];
	SViewEntity& viewEntity = m_spotLightViewEntities[m_lastSpotLightIndex];
	lightEntity.m_lightIndex = m_lastSpotLightIndex;
	
	m_lastSpotLightIndex++;

	SpotLightComponent* spotLight = new SpotLightComponent(
		componentName, componentID, localPosition, localAngle,
		lightPower, fallOffStart, fallOffEnd, spotPower,
		&lightEntity, &m_spotLightEntityBuffer,
		&viewEntity, &m_spotLightViewEntityBuffer,
		fovAngle
	);

	m_spotLights.push_back(spotLight);

	return spotLight;
}

PointLightComponent* LightManager::CreatePointLight(
	const string& componentName, 
	const uint32_t& componentID, 
	const XMFLOAT3& localPosition, 
	const float& lightPower, 
	const float& fallOffStart, 
	const float& fallOffEnd
)
{
	SLightEntity& lightEntity = m_pointLightEntities[m_lastPointLightIndex];
	array<SViewEntity, 6>& viewCubeEntity = m_pointLightViewCubeEntities[m_lastPointLightIndex];
	lightEntity.m_lightIndex = m_lastPointLightIndex;

	m_lastPointLightIndex++;

	PointLightComponent* pointLight = new PointLightComponent(
		componentName, componentID,
		localPosition, lightPower, fallOffStart, fallOffEnd,
		&lightEntity, &m_pointLightEntityBuffer,
		&viewCubeEntity, &m_pointLightViewCubeEntityBuffer
	);

	m_pointLights.push_back(pointLight);

	return pointLight;
}
