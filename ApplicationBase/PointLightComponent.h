#pragma once
#include "AComponent.h"
#include "LightEntity.h"

class PointLightComponent : public AComponent, public LightEntity
{
public:
	PointLightComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale
	);
	~PointLightComponent() override;

public:


public:
	virtual void Accept(IComponentVisitor* visitor) override;
};

