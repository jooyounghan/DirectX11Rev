#include "LightManager.h"
#include "SpotLightComponent.h"
#include "PointLightComponent.h"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

LightManager::LightManager()
	: 
	m_spotLightDepthTestViews(
		GDefaultShadowMapWidth, GDefaultShadowMapHeight, 
		MaxSpotLightCount, 1, NULL, NULL, D3D11_USAGE_DEFAULT, 
		DXGI_FORMAT_R32_TYPELESS, D3D11_BIND_DEPTH_STENCIL
	),
	m_pointLightDepthTestViews(
		GDefaultShadowMapWidth, GDefaultShadowMapHeight,
		MaxPointLightCount * 6, 1, NULL, D3D11_RESOURCE_MISC_TEXTURECUBE, D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_TYPELESS, D3D11_BIND_DEPTH_STENCIL
	),
	m_spotLightEntities(new array<SLightEntity, MaxSpotLightCount>()),
	m_spotLightViewEntities(new array<SViewEntity, MaxSpotLightCount>()),
	m_pointLightEntities(new array<SLightEntity, MaxPointLightCount>()),
	m_pointLightPositions(new array<XMVECTOR, MaxPointLightCount>()),
	m_spotLightEntitiesBuffer(sizeof(SLightEntity), static_cast<UINT>(m_spotLightEntities->size()), m_spotLightEntities->data()),
	m_spotLightViewEntitiesBuffer(sizeof(SViewEntity), static_cast<UINT>(m_spotLightViewEntities->size()), m_spotLightViewEntities->data()),
	m_pointLightEntitiesBuffer(sizeof(SLightEntity), static_cast<UINT>(m_pointLightEntities->size()), m_pointLightEntities->data()),
	m_pointLightPositionsBuffer(sizeof(XMVECTOR), static_cast<UINT>(m_pointLightPositions->size()), m_pointLightPositions->data()),
	m_lightManagerEntityBuffer(sizeof(SLightManagerEntity), 1, &m_lightManagerEntity)
{
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
		&lightEntity, &m_spotLightEntitiesBuffer,
		&viewEntity, &m_spotLightViewEntitiesBuffer,
		fovAngle, m_spotLightDSVs[m_lastSpotLightIndex].GetAddressOf()
	);

	m_lastSpotLightIndex++;
	m_spotLights.push_back(spotLight);
	
	m_lightManagerEntity.m_spotLightCount++;	
	m_isLightCountChanged.store(true);
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
	XMVECTOR& lightPosition = m_pointLightPositions->at(m_lastPointLightIndex);

	PointLightComponent* pointLight = new PointLightComponent(
		componentName, componentID,
		localPosition, lightPower, fallOffStart, fallOffEnd, m_lastPointLightIndex,
		&lightEntity, &m_pointLightEntitiesBuffer,
		&lightPosition, &m_pointLightPositionsBuffer,
		{
			m_pointLightCubeDSVs[m_lastPointLightIndex][0].GetAddressOf(),
			m_pointLightCubeDSVs[m_lastPointLightIndex][1].GetAddressOf(),
			m_pointLightCubeDSVs[m_lastPointLightIndex][2].GetAddressOf(),
			m_pointLightCubeDSVs[m_lastPointLightIndex][3].GetAddressOf(),
			m_pointLightCubeDSVs[m_lastPointLightIndex][4].GetAddressOf(),
			m_pointLightCubeDSVs[m_lastPointLightIndex][5].GetAddressOf()
		}
	);

	m_lastPointLightIndex++;
	m_pointLights.push_back(pointLight);

	m_lightManagerEntity.m_pointLightCount++;
	m_isLightCountChanged.store(true);
	return pointLight;
}
