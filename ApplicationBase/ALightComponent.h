#pragma once
#include "AComponent.h"

class DynamicBuffer;

struct SLightEntity
{
	float m_lightPower;
	float m_fallOffStart;
	float m_fallOffEnd;
	float m_spotPower;
};

class ALightComponent : public AComponent
{
public:
	ALightComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const DirectX::XMFLOAT3& localAngle
	);
	~ALightComponent() override = default;

protected:
	SLightEntity m_lightEntity;
	DynamicBuffer* m_lightEntityBuffer;

public:
	inline DynamicBuffer* GetLightEntityBuffer() const { m_lightEntityBuffer; }
};

