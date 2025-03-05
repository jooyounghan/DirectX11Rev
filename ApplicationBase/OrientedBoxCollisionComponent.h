#pragma once
#include "ACollisionComponent.h"
#include "ACollidableOrientedBox.h"

class OrientedBoxCollisionComponent : public ACollisionComponent, public ACollidableOrientedBox
{
public:
	OrientedBoxCollisionComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const DirectX::XMFLOAT3& localAngle,
		const DirectX::XMFLOAT3& scale,
		const DirectX::XMFLOAT3& extents
	);
	~OrientedBoxCollisionComponent() override;

protected:
	DirectX::XMVECTOR m_extents;

public:
	void SetExtents(const DirectX::XMVECTOR& extents);
	const DirectX::XMVECTOR& GetExtents() { return m_extents; }
	virtual const DirectX::XMMATRIX& GetAbsoluteTranformation() const;

public:
	virtual DirectX::XMMATRIX GetLocalTransformation() const override;
	virtual bool GetDefaultRenderable() const override;
	virtual void Accept(IComponentVisitor* visitor) override;
	virtual void UpdateEntity(ID3D11DeviceContext* deviceContext);

public:
	virtual void SetCollisionOption(ICollisionOption* collisionOption) override;
	virtual void OnCollide(ICollisionAcceptor* accpetor) override;

protected:
	virtual void UpdateBoundingProperty() override;
	virtual void UpdateBoundingVolumeHierarchy() override;
};

