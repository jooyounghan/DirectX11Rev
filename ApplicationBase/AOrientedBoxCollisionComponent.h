#pragma once
#include "AComponent.h"
#include "CollidableOrientedBox.h"

class ARenderOrientedBoxCollisionComponent : public AComponent, public CollidableOrientedBox
{
public:
	ARenderOrientedBoxCollisionComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const DirectX::XMFLOAT3& localAngle,
		const DirectX::XMFLOAT3& extension
	);

public:
	virtual void Accept(IComponentVisitor* visitor) override;
	virtual void OnCollide(ACollisionAcceptor*) = 0;
};

