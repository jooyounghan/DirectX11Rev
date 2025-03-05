#pragma once
#include "LightComponent.h"
#include "ViewEntity.h"
#include "StructuredBuffer.h"
#include "DynamicBuffer.h"

#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "DSVOption.h"

#include <vector>
#include <array>
#include <string>

constexpr size_t MaxSpotLightCount = 1024;
constexpr size_t MaxPointLightCount = 256;

class SpotLightComponent;
class PointLightComponent;

struct SLightManagerEntity
{
	uint32_t m_spotLightCount;
	uint32_t m_pointLightCount;
	uint32_t dummy[2];
};

class LightManager
{
public:
	LightManager();
	~LightManager();

protected:
	std::vector<SpotLightComponent*> m_spotLights;
	std::vector<PointLightComponent*> m_pointLights;

public:
	inline const std::vector<SpotLightComponent*>& GetSpotLights() const { return m_spotLights; }
	inline const std::vector<PointLightComponent*>& GetPointLights() const { return m_pointLights; }

protected:
	Texture2DInstance<SRVOption>																	m_spotLightDepthTestViews;
	std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>, MaxSpotLightCount>					m_spotLightDSVs;
	Texture2DInstance<SRVOption>																	m_pointLightDepthTestViews;
	std::array<std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>, 6>, MaxSpotLightCount>	m_pointLightCubeDSVs;

public:
	inline Texture2DInstance<SRVOption>& GetSpotLightDepthTestViews() { return m_spotLightDepthTestViews; }
	inline std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>, MaxSpotLightCount>& GetSpotLightDSVs() { return m_spotLightDSVs; }
	inline Texture2DInstance<SRVOption>& GetPointLightDepthTestViews() { return m_pointLightDepthTestViews; }
	inline std::array<std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>, 6>, MaxSpotLightCount>& GetPointLightCubeDSVs() { return m_pointLightCubeDSVs; }

protected:
	SLightManagerEntity m_lightManagerEntity;
	DynamicBuffer		m_lightManagerEntityBuffer;

protected:
	std::atomic_bool	m_isLightCountChanged = false;

public:
	inline DynamicBuffer& GetLightManagerEntityBuffer() { return m_lightManagerEntityBuffer; }

public:
	inline bool ConsumeIsLightCountChanged() { return m_isLightCountChanged.exchange(false); }
	inline void SetIsLightCountChanged(const bool& isLightCountChanged) { m_isLightCountChanged = isLightCountChanged; }

public:
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
	std::array<SLightEntity, MaxSpotLightCount>*	m_spotLightEntities;
	std::array<SViewEntity, MaxSpotLightCount>*		m_spotLightViewEntities;
	StructuredBuffer								m_spotLightEntitiesBuffer;
	StructuredBuffer								m_spotLightViewEntitiesBuffer;

public:
	inline StructuredBuffer& GetSpotLightEntitiesBuffer() { return m_spotLightEntitiesBuffer; }
	inline StructuredBuffer& GetSpotLightViewEntitiesBuffer() { return m_spotLightViewEntitiesBuffer; }


protected:
	std::array<SLightEntity, MaxPointLightCount>*		m_pointLightEntities;
	std::array<DirectX::XMVECTOR, MaxPointLightCount>*	m_pointLightPositions;
	StructuredBuffer									m_pointLightEntitiesBuffer;
	StructuredBuffer									m_pointLightPositionsBuffer;

public:
	inline StructuredBuffer& GetPointLightEntitiesBuffer() { return m_pointLightEntitiesBuffer; }
	inline StructuredBuffer& GetPointLightPositionsBuffer() { return m_pointLightPositionsBuffer; }

protected:
	uint32_t m_lastSpotLightIndex = 0;
	uint32_t m_lastPointLightIndex = 0;
};

