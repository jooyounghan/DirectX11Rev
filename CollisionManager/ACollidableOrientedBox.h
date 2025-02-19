#pragma once
#include "ICollisionAcceptor.h"

class ACollidableOrientedBox : public ICollisionAcceptor, public DirectX::BoundingOrientedBox
{
public:
	ACollidableOrientedBox() = default;
	~ACollidableOrientedBox() override = default;

public:
	virtual bool Accept(ICollisionVisitor& collisionVisitor) const override;
	virtual bool IsIntersectBoundingBox(const DirectX::BoundingBox& boundingBox) const override;

public:
	virtual DirectX::BoundingBox GetBoundingBox(const float& margin) const override;
};

