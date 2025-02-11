#pragma once
#include "CollisionVisitor.h"

class IntersectVisitor : public ACollisionVisitor
{
public:
	IntersectVisitor(ICollisionAcceptor* collisionAcceptor);
	~IntersectVisitor() override = default;

public:
	virtual bool Visit(const ACollidableSphere* const collidableSphere) const override;
	virtual bool Visit(const ACollidableOrientedBox* const collidableOrientedBox)const override;
	virtual bool Visit(const ACollidableFrustum* const collidableFrustum) const override;
};

class IntersectSphereVisitor : public ACollisionSpecifiedVisitor<ACollidableSphere>
{
public:
	IntersectSphereVisitor(const ACollidableSphere* const collidableSphere);
	~IntersectSphereVisitor() override = default;

public:
	virtual bool Visit(const ACollidableSphere* const collidableSphere) const override;
	virtual bool Visit(const ACollidableOrientedBox* const collidableOrientedBox) const override;
	virtual bool Visit(const ACollidableFrustum* const collidableFrustum) const override;
};

class IntersectOrientedBoxVisitor : public ACollisionSpecifiedVisitor<ACollidableOrientedBox>
{
public:
	IntersectOrientedBoxVisitor(const ACollidableOrientedBox* const collidableOrientedBox);
	~IntersectOrientedBoxVisitor() override = default;

public:
	virtual bool Visit(const ACollidableSphere* const collidableSphere) const override;
	virtual bool Visit(const ACollidableOrientedBox* const collidableOrientedBox) const override;
	virtual bool Visit(const ACollidableFrustum* const collidableFrustum) const override;
};


class IntersectFrustumVisitor : public ACollisionSpecifiedVisitor<ACollidableFrustum>
{
public:
	IntersectFrustumVisitor(const ACollidableFrustum* const collidableFrustum);
	~IntersectFrustumVisitor() override = default;

public:
	virtual bool Visit(const ACollidableSphere* const collidableSphere) const override;
	virtual bool Visit(const ACollidableOrientedBox* const collidableOrientedBox) const override;
	virtual bool Visit(const ACollidableFrustum* const collidableFrustum) const override;
};
