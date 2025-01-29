#pragma once
#include "ICollisionAcceptor.h"

class CollidableFrustum : public ICollisionAcceptor, public DirectX::BoundingFrustum
{
public:
	CollidableFrustum() = default;
	~CollidableFrustum() override = default;

public:
	virtual bool Accept(ICollisionVisitor& collisionVisitor) const override;
	virtual bool IsInBVNode(BoundingVolumeNode* boundingVolumeNode) const override;

public:
	virtual DirectX::BoundingBox GetBoundingBox(const float& margin) const override;
	void SetBoundingProperties(
		const DirectX::XMFLOAT3& origin,
		const DirectX::XMFLOAT4& rotationQuaternion,
		const float& fovAngle,
		const float& nearZ,
		const float& farZ
	);
};

