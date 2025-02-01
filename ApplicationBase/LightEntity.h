#pragma once
#include <unordered_map>

class DynamicBuffer;

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

public:
	static std::unordered_map<uint32_t, LightEntity*> Lights;

protected:
	SLightEntity m_lightEntity;
	DynamicBuffer* m_lightEntityBuffer = nullptr;

public:
	inline const SLightEntity& GetLightEntity() { return m_lightEntity; }
	void SetLigthEntity(const float& lightPower, const float& fallOffStart, const float& fallOffEnd, const float& spotPower);

public:
	inline DynamicBuffer* GetLightEntityBuffer() const { m_lightEntityBuffer; }
};

