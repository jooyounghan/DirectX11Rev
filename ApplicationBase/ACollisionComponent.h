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
		const DirectX::XMFLOAT3& localAngle
	);
	virtual ~ACollisionComponent() override;

protected:
	ICollisionOption* m_collisionOption = nullptr;

public:
	virtual void SetCollisionOption(ID3D11Device* device, ICollisionOption* collisionOption);
	virtual void UpdateBoundingVolumeHierarchy(ID3D11Device* device, ID3D11DeviceContext* deviceContext) = 0;
};

