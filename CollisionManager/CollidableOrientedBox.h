#pragma once
#include "ACollisionAcceptor.h"

class CollidableOrientedBox : public ACollisionAcceptor, public DirectX::BoundingOrientedBox
{
public:
	CollidableOrientedBox() = default;
	~CollidableOrientedBox() override = default;

public:
	virtual bool Accept(ICollisionVisitor& collisionVisitor) const override;
	virtual bool IsInBVNode(BoundingVolumeNode* boundingVolumeNode) const override;

public:
	virtual DirectX::BoundingBox GetBoundingBox(const float& margin) const override;
	void SetBoundingProperties(
		const DirectX::XMFLOAT3& center,
		const DirectX::XMFLOAT3& extents,
		const DirectX::XMFLOAT4& rotationQuaternion
	);
};

