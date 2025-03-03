#pragma once
#include "AComponent.h"

class ComponentPSOManager;
class StructuredBuffer;

constexpr uint32_t GDefaultShadowMapWidth = 512;
constexpr uint32_t GDefaultShadowMapHeight = 512;

struct SLightEntity
{
	float m_lightPower = 0.f;
	float m_fallOffStart = 0.f;
	float m_fallOffEnd = 0.f;
	float m_spotPower = 0.f;
};

class LightComponent : public AComponent
{
public:
	LightComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const DirectX::XMFLOAT3& localAngle,
		const float& lightPower,
		const float& fallOffStart,
		const float& fallOffEnd,
		const float& spotPower,
		const uint32_t& lightIdx,
		SLightEntity* lightEntityCached,
		StructuredBuffer* lightEntityCachedBuffer
	);
	virtual ~LightComponent() = default;

protected:
	SLightEntity* m_lightEntityCached = nullptr;
	StructuredBuffer* m_lightEntityCachedBuffer = nullptr;
	AtomicFlag m_viewEntityCachedFlag = false;

public:
	inline SLightEntity* GetLightEntityAddress() { return m_lightEntityCached; }
	inline StructuredBuffer* GetLightEntityBufferAddress() { return m_lightEntityCachedBuffer; }
	inline AtomicFlag& GetViewUpdatedFlag() { return m_viewEntityCachedFlag; }

public:
	void UpdateLightEntity(
		const float& lightPower,
		const float& fallOffStart,
		const float& fallOffEnd,
		const float& spotPower
	);

public:
	virtual void GenerateShadowMap(
		ID3D11DeviceContext* deviceContext,
		ComponentPSOManager* componentPSOManager,
		const std::vector<AComponent*>& components
	) = 0;
};