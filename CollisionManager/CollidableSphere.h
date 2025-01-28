#pragma once
#include "ACollisionAcceptor.h"

class CollidableSphere : public ACollisionAcceptor, public DirectX::BoundingSphere
{
public:
	CollidableSphere() = default;
	~CollidableSphere() override = default;

public:
	virtual bool Accept(ICollisionVisitor& collisionVisitor) const override;
	virtual bool IsInBVNode(BoundingVolumeNode* boundingVolumeNode) const override;

public:
	virtual DirectX::BoundingBox GetBoundingBox(const float& margin) const override;
	void SetBoundingProperties(
		const DirectX::XMFLOAT3& center,
		const float& radius
	);
};

