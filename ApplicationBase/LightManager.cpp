#include "LightManager.h"
#include "SpotLightComponent.h"
#include "PointLightComponent.h"

using namespace std;
using namespace DirectX;

LightManager::LightManager(ID3D11Device* const device)
	: 
	m_spotLightEntities(new array<SLightEntity, MaxSpotLightCount>()),
	m_spotLightViewEntities(new array<SViewEntity, MaxSpotLightCount>()),
	m_pointLightEntities(new array<SLightEntity, MaxPointLightCount>()),
	m_pointLightXViewEntities(new array<SViewEntity, MaxPointLightCount>()),
	m_pointLightNegativeXViewEntities(new array<SViewEntity, MaxPointLightCount>()),
	m_pointLightYViewEntities(new array<SViewEntity, MaxPointLightCount>()),
	m_pointLightNegativeYViewEntities(new array<SViewEntity, MaxPointLightCount>()),
	m_pointLightZViewEntities(new array<SViewEntity, MaxPointLightCount>()),
	m_pointLightNegativeZViewEntities(new array<SViewEntity, MaxPointLightCount>()),

	m_spotLightEntityBuffer(sizeof(SLightEntity), static_cast<UINT>(m_spotLightEntities->size()), m_spotLightEntities->data()),
	m_spotLightViewEntityBuffer(sizeof(SViewEntity), static_cast<UINT>(m_spotLightViewEntities->size()), m_spotLightViewEntities->data()),
	m_pointLightEntityBuffer(sizeof(SLightEntity), static_cast<UINT>(m_pointLightEntities->size()), m_pointLightEntities->data()),
	m_pointLightXViewEntityBuffer(sizeof(SViewEntity), static_cast<UINT>(m_pointLightXViewEntities->size()), m_pointLightXViewEntities->data()),
	m_pointLightNegativeXViewEntityBuffer(sizeof(SViewEntity), static_cast<UINT>(m_pointLightNegativeXViewEntities->size()), m_pointLightNegativeXViewEntities->data()),
	m_pointLightYViewEntityBuffer(sizeof(SViewEntity), static_cast<UINT>(m_pointLightYViewEntities->size()), m_pointLightYViewEntities->data()),
	m_pointLightNegativeYViewEntityBuffer(sizeof(SViewEntity), static_cast<UINT>(m_pointLightNegativeYViewEntities->size()), m_pointLightNegativeYViewEntities->data()),
	m_pointLightZViewEntityBuffer(sizeof(SViewEntity), static_cast<UINT>(m_pointLightZViewEntities->size()), m_pointLightZViewEntities->data()),
	m_pointLightNegativeZViewEntityBuffer(sizeof(SViewEntity), static_cast<UINT>(m_pointLightNegativeZViewEntities->size()), m_pointLightNegativeZViewEntities->data())
{
	m_spotLightEntityBuffer.InitializeBuffer(device);
	m_spotLightViewEntityBuffer.InitializeBuffer(device);
	m_pointLightEntityBuffer.InitializeBuffer(device);

	m_pointLightXViewEntityBuffer.InitializeBuffer(device);
	m_pointLightNegativeXViewEntityBuffer.InitializeBuffer(device);
	m_pointLightYViewEntityBuffer.InitializeBuffer(device);
	m_pointLightNegativeYViewEntityBuffer.InitializeBuffer(device);
	m_pointLightZViewEntityBuffer.InitializeBuffer(device);
	m_pointLightNegativeZViewEntityBuffer.InitializeBuffer(device);
}

LightManager::~LightManager()
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
	SLightEntity& lightEntity = m_spotLightEntities->at(m_lastSpotLightIndex);
	SViewEntity& viewEntity = m_spotLightViewEntities->at(m_lastSpotLightIndex);


	SpotLightComponent* spotLight = new SpotLightComponent(
		componentName, componentID, localPosition, localAngle,
		lightPower, fallOffStart, fallOffEnd, spotPower, m_lastSpotLightIndex,
		&lightEntity, &m_spotLightEntityBuffer,
		&viewEntity, &m_spotLightViewEntityBuffer,
		fovAngle
	);

	m_lastSpotLightIndex++;
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
	SLightEntity& lightEntity = m_pointLightEntities->at(m_lastPointLightIndex);
	;
	array<SViewEntity*, 6> viewEntities =
	{
		&m_pointLightXViewEntities->at(m_lastPointLightIndex),
		&m_pointLightNegativeXViewEntities->at(m_lastPointLightIndex),
		&m_pointLightYViewEntities->at(m_lastPointLightIndex),
		&m_pointLightNegativeYViewEntities->at(m_lastPointLightIndex),
		&m_pointLightZViewEntities->at(m_lastPointLightIndex),
		&m_pointLightNegativeZViewEntities->at(m_lastPointLightIndex)
	};
	array<StructuredBuffer*, 6> viewEntityBuffers =
	{
		&m_pointLightXViewEntityBuffer,
		&m_pointLightNegativeXViewEntityBuffer,
		&m_pointLightYViewEntityBuffer,
		&m_pointLightNegativeYViewEntityBuffer,
		&m_pointLightZViewEntityBuffer,
		&m_pointLightNegativeZViewEntityBuffer
	};

	PointLightComponent* pointLight = new PointLightComponent(
		componentName, componentID,
		localPosition, lightPower, fallOffStart, fallOffEnd, m_lastPointLightIndex,
		&lightEntity, &m_pointLightEntityBuffer,
		viewEntities, viewEntityBuffers
	);

	m_lastPointLightIndex++;
	m_pointLights.push_back(pointLight);
	return pointLight;
}
