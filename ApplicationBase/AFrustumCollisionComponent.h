#pragma once
#include "AComponent.h"
#include "CollidableFrustum.h"

class ARenderFrustumCollisionComponent : public AComponent, public CollidableFrustum
{
public:
	ARenderFrustumCollisionComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const DirectX::XMFLOAT3& localAngle,
		const float& fovAngle,
		const float& nearZ,
		const float& farZ
	);

public:
	virtual void Accept(IComponentVisitor* visitor) override;
	virtual void OnCollide(ACollisionAcceptor*) = 0;
};

