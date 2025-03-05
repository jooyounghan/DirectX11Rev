#pragma once
#include "ICollisionAcceptor.h"

class ACollidableFrustum : public ICollisionAcceptor, public DirectX::BoundingFrustum
{
public:
	ACollidableFrustum() = default;
	~ACollidableFrustum() override = default;

public:
	virtual bool Accept(ICollisionVisitor& collisionVisitor) const override;
	virtual bool IsIntersectBoundingBox(const DirectX::BoundingBox& boundingBox) const override;
	virtual bool IsContainedByBoundingBox(const DirectX::BoundingBox& boundingBox) const override;

public:
	virtual DirectX::BoundingBox GetBoundingBox(const float& margin) const override;
};

