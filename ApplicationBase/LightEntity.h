#pragma once
#include <unordered_map>

struct ID3D11DeviceContext;
class DynamicBuffer;
class ComponentPSOManager;
class AComponent;

constexpr uint32_t GDefaultShadowMapWidth = 512;
constexpr uint32_t GDefaultShadowMapHeight = 512;

struct SLightEntity
{
	float m_lightPower = 0.f;
	float m_fallOffStart = 0.f;
	float m_fallOffEnd = 0.f;
	float m_spotPower = 0.f;
};

class LightEntity
{
public:
	LightEntity() = default;
	virtual ~LightEntity() = default;

protected:
	SLightEntity m_lightEntity;
	DynamicBuffer* m_lightEntityBuffer = nullptr;

public:
	inline const SLightEntity& GetLightEntity() { return m_lightEntity; }
	void SetLigthEntity(const float& lightPower, const float& fallOffStart, const float& fallOffEnd, const float& spotPower);

public:
	inline DynamicBuffer* GetLightEntityBuffer() const { m_lightEntityBuffer; }

public:
	virtual void GenerateShadowMap(
		ID3D11DeviceContext* const* deviceContextAddress,
		ComponentPSOManager* componentPsoManager,
		const std::vector<AComponent*>& components
	) = 0;
};