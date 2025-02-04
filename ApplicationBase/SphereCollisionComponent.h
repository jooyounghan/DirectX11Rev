#pragma once
#include "ACollisionComponent.h"
#include "CollidableSphere.h"

class SphereCollisionComponent : public ACollisionComponent, public CollidableSphere
{
public:
	SphereCollisionComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const float& radius
	);
	~SphereCollisionComponent() override;

public:
	virtual bool GetDefaultRenderable() override;

public:
	virtual void Accept(IComponentVisitor* visitor) override;

public:
	virtual void UpdateAbsoluteEntities() override;
	virtual void UpdateComponentTransformation() override;

public:
	virtual void SetCollisionOption(ICollisionOption* collisionOption) override;
	virtual void UpdateBoundingVolumeHierarchy() override;
	virtual void OnCollide(ICollisionAcceptor* accpetor) override;
};

