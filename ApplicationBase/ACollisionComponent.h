#pragma once
#include "AComponent.h"

class ICollisionOption;
class ICollisionAcceptor;

class ACollisionComponent : public AComponent
{
public:
	ACollisionComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const DirectX::XMFLOAT3& localAngle,
		const DirectX::XMFLOAT3& scale
	);
	~ACollisionComponent() override;

protected:
	ICollisionOption* m_collisionOption = nullptr;

public:
	virtual void SetCollisionOption(ICollisionOption* collisionOption);
	virtual void UpdateBoundingVolumeHierarchy() = 0;
};

