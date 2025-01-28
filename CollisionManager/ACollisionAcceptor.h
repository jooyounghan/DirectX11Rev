#pragma once
#include <DirectXCollision.h>

class ICollisionVisitor;
class BoundingVolumeNode;

class ACollisionAcceptor
{
public:
	virtual ~ACollisionAcceptor() = default;

public: 
	virtual bool Accept(ICollisionVisitor& collisionVisitor) const = 0;
	virtual bool IsInBVNode(BoundingVolumeNode* boundingVolumeNode) const = 0;

public:
	virtual DirectX::BoundingBox GetBoundingBox(const float& margin) const = 0;

public:
	virtual void UpdateBoundginVolumeHierachy() = 0;
	virtual void OnCollide(ACollisionAcceptor*) = 0;
};

