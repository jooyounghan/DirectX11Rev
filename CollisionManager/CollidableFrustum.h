#pragma once
#include "ICollisionAcceptor.h"

class CollidableFrustum : public ICollisionAcceptor, public DirectX::BoundingFrustum
{
public:
	CollidableFrustum() = default;
	~CollidableFrustum() override = default;

public:
	virtual bool Accept(ICollisionVisitor& collisionVisitor) const override;
	virtual bool IsIntersectBoundingBox(const DirectX::BoundingBox& boundingBox) const override;

public:
	virtual DirectX::BoundingBox GetBoundingBox(const float& margin) const override;
};

