#pragma once
#include "ICollisionAcceptor.h"

class ACollidableSphere : public ICollisionAcceptor, public DirectX::BoundingSphere
{
public:
	ACollidableSphere() = default;
	~ACollidableSphere() override = default;

public:
	virtual bool Accept(ICollisionVisitor& collisionVisitor) const override;
	virtual bool IsIntersectBoundingBox(const DirectX::BoundingBox& boundingBox) const override;

public:
	virtual DirectX::BoundingBox GetBoundingBox(const float& margin) const override;
};

