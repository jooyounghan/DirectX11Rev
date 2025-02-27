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
constexpr size_t MaxPointLightCount = 1024;

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
	Texture2DInstance<SRVOption> m_spotLightDepthTestViews;
	std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>, MaxSpotLightCount> m_spotLightDSVs;

public:
	inline Texture2DInstance<SRVOption>& GetSpotLightDepthTestViews() { return m_spotLightDepthTestViews; }
	inline std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>, MaxSpotLightCount>& GetSpotLightDSVs() { return m_spotLightDSVs; }

protected:
	SLightManagerEntity m_lightManagerEntity;
	DynamicBuffer m_lightManagerEntityBuffer;
	std::atomic_bool m_isLightCountChanged = false;

public:
	inline DynamicBuffer& GetLightManagerEntityBuffer() { return m_lightManagerEntityBuffer; }
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
	std::array<SLightEntity, MaxSpotLightCount>* m_spotLightEntities;
	StructuredBuffer m_spotLightEntityBuffer;
	std::array<SViewEntity, MaxSpotLightCount>* m_spotLightViewEntities;
	StructuredBuffer m_spotLightViewEntityBuffer;
	uint32_t m_lastSpotLightIndex = 0;

public:
	inline StructuredBuffer& GetSpotLightEntityBuffer() { return m_spotLightEntityBuffer; }
	inline StructuredBuffer& GetSpotLightViewEntityBuffer() { return m_spotLightViewEntityBuffer; }

protected:
	std::array<SLightEntity, MaxPointLightCount>* m_pointLightEntities;
	StructuredBuffer m_pointLightEntityBuffer;
	std::array<SViewEntity, MaxPointLightCount>* m_pointLightXViewEntities;
	std::array<SViewEntity, MaxPointLightCount>* m_pointLightNegativeXViewEntities;
	std::array<SViewEntity, MaxPointLightCount>* m_pointLightYViewEntities;
	std::array<SViewEntity, MaxPointLightCount>* m_pointLightNegativeYViewEntities;
	std::array<SViewEntity, MaxPointLightCount>* m_pointLightZViewEntities;
	std::array<SViewEntity, MaxPointLightCount>* m_pointLightNegativeZViewEntities;
	StructuredBuffer m_pointLightXViewEntityBuffer;
	StructuredBuffer m_pointLightNegativeXViewEntityBuffer;
	StructuredBuffer m_pointLightYViewEntityBuffer;
	StructuredBuffer m_pointLightNegativeYViewEntityBuffer;
	StructuredBuffer m_pointLightZViewEntityBuffer;
	StructuredBuffer m_pointLightNegativeZViewEntityBuffer;
	uint32_t m_lastPointLightIndex = 0;

public:
	inline StructuredBuffer& GetPointLightEntityBuffer() { return m_pointLightEntityBuffer; }
	inline StructuredBuffer& GetPointLightXViewEntityBuffer() { return m_pointLightXViewEntityBuffer; }
	inline StructuredBuffer& GetPointLightNegativeXViewEntityBuffer() { return m_pointLightNegativeXViewEntityBuffer; }
	inline StructuredBuffer& GetPointLightYViewEntityBuffer() { return m_pointLightYViewEntityBuffer; }
	inline StructuredBuffer& GetPointLightNegativeYViewEntityBuffer() { return m_pointLightNegativeYViewEntityBuffer; }
	inline StructuredBuffer& GetPointLightZViewEntityBuffer() { return m_pointLightZViewEntityBuffer; }
	inline StructuredBuffer& GetPointLightNegativeZViewEntityBuffer() { return m_pointLightNegativeZViewEntityBuffer; }
};

