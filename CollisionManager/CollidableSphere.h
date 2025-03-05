#pragma once
#include "ICollisionAcceptor.h"

class CollidableSphere : public ICollisionAcceptor, public DirectX::BoundingSphere
{
public:
	CollidableSphere() = default;
	~CollidableSphere() override = default;

public:
	virtual bool Accept(ICollisionVisitor& collisionVisitor) const override;
	virtual bool IsIntersectBoundingBox(const DirectX::BoundingBox& boundingBox) const override;
	virtual bool IsContainedByBoundingBox(const DirectX::BoundingBox& boundingBox) const override;

public:
	virtual DirectX::BoundingBox GetBoundingBox(const float& margin) const override;
	void SetBoundingProperties(
		const DirectX::XMVECTOR& center,
		const float& radius
	);
};

