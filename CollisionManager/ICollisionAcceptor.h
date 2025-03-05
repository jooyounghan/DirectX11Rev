#pragma once
#include <DirectXCollision.h>

class ICollisionVisitor;
class BoundingVolumeNode;

class ICollisionAcceptor
{
public:
	virtual ~ICollisionAcceptor() = default;

public: 
	virtual bool Accept(ICollisionVisitor& collisionVisitor) const = 0;
	virtual bool IsIntersectBoundingBox(const DirectX::BoundingBox& boundingBox) const = 0;
	virtual bool IsContainedByBoundingBox(const DirectX::BoundingBox& boundingBox) const = 0;

public:
	virtual DirectX::BoundingBox GetBoundingBox(const float& margin) const = 0;
	virtual void OnCollide(ICollisionAcceptor*) = 0;

protected:
	virtual void UpdateBoundingProperty() = 0;
};

