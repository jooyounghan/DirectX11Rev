#pragma once
#include "StructuredBuffer.h"
#include "LightComponent.h"
#include "ViewEntity.h"

#include <vector>
#include <array>
#include <string>

constexpr size_t MaxSpotLightCount = 1024;
constexpr size_t MaxPointLightCount = 1024;

class SpotLightComponent;
class PointLightComponent;

class LightManager
{
public:
	LightManager();
	~LightManager() = default;

protected:
	std::vector<SpotLightComponent*> m_spotLights;
	std::vector<PointLightComponent*> m_pointLights;

public:
	inline const std::vector<SpotLightComponent*>& GetSpotLights() const { return m_spotLights; }
	inline const std::vector<PointLightComponent*>& GetPointLights() const { return m_pointLights; }

	SpotLightComponent* CreateSpotLight(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const DirectX::XMFLOAT3& localAngle,
		const float& lightPower,
		const float& fallOffStart,
		const float& fallOffEnd,
		const float& spotPower,
		const float& fovAngle
	);
	PointLightComponent* CreatePointLight(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const float& lightPower,
		const float& fallOffStart,
		const float& fallOffEnd
	);

protected:
	std::array<SLightEntity, MaxSpotLightCount> m_spotLightEntities;
	StructuredBuffer m_spotLightEntityBuffer;
	std::array<SViewEntity, MaxSpotLightCount> m_spotLightViewEntities;
	StructuredBuffer m_spotLightViewEntityBuffer;
	uint32_t m_lastSpotLightIndex = 0;

	std::array<SLightEntity, MaxPointLightCount> m_pointLightEntities;
	StructuredBuffer m_pointLightEntityBuffer;
	std::array<std::array<SViewEntity, 6>, MaxPointLightCount> m_pointLightViewCubeEntities;
	StructuredBuffer m_pointLightViewCubeEntityBuffer;
	uint32_t m_lastPointLightIndex = 0;

	// LightComponent에 OnDispose 함수를 통한 연결 필요

public:

};

