#pragma once
#include "ACollisionComponent.h"
#include "ACollidableSphere.h"

class SphereCollisionComponent : public ACollisionComponent, public ACollidableSphere
{
public:
	SphereCollisionComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const DirectX::XMFLOAT3& scale,
		const float& radius
	);
	~SphereCollisionComponent() override;

protected:
	float m_radius;

public:
	void SetRadius(const float& radius);
	inline const float& GetRadius() { return m_radius; }
	virtual const DirectX::XMMATRIX& GetAbsoluteTranformation() const;

public:
	virtual DirectX::XMMATRIX GetLocalTransformation() const override;
	virtual bool GetDefaultRenderable() const override;
	virtual void Accept(IComponentVisitor* visitor) override;

public:
	virtual void SetCollisionOption(ICollisionOption* collisionOption) override;
	virtual void UpdateBoundingVolumeHierarchy() override;
	virtual void UpdateBoundingProperty() override;
	virtual void OnCollide(ICollisionAcceptor* accpetor) override;
};

