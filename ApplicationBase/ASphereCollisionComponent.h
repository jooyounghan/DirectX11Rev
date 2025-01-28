#pragma once
#include "AComponent.h"
#include "CollidableSphere.h"

class ARenderSphereCollisionComponent : public AComponent, public CollidableSphere
{
public:
	ARenderSphereCollisionComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const float& radius
	);

public:
	virtual void Accept(IComponentVisitor* visitor) override;
};

