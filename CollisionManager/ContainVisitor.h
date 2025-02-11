#pragma once
#include "CollisionVisitor.h"

class ContainVisitor : public ACollisionVisitor
{
public:
	ContainVisitor(ICollisionAcceptor* collisionAcceptor);
	~ContainVisitor() override = default;

public:
	virtual bool Visit(const ACollidableSphere* const collidableSphere) const override;
	virtual bool Visit(const ACollidableOrientedBox* const collidableOrientedBox) const override;
	virtual bool Visit(const ACollidableFrustum* const collidableFrustum) const override;
};


class ContainSphereVisitor : public ACollisionSpecifiedVisitor<ACollidableSphere>
{
public:
	ContainSphereVisitor(const ACollidableSphere* const collidableSphere);
	~ContainSphereVisitor() override = default;

public:
	virtual bool Visit(const ACollidableSphere* const collidableSphere) const override;
	virtual bool Visit(const ACollidableOrientedBox* const collidableOrientedBox) const override;
	virtual bool Visit(const ACollidableFrustum* const collidableFrustum) const override;
};

class ContainOrientedBoxVisitor : public ACollisionSpecifiedVisitor<ACollidableOrientedBox>
{
public:
	ContainOrientedBoxVisitor(const ACollidableOrientedBox* const collidableOrientedBox);
	~ContainOrientedBoxVisitor() override = default;


public:
	virtual bool Visit(const ACollidableSphere* const collidableSphere) const override;
	virtual bool Visit(const ACollidableOrientedBox* const collidableOrientedBox) const override;
	virtual bool Visit(const ACollidableFrustum* const collidableFrustum) const override;
};


class ContainFrustumVisitor : public ACollisionSpecifiedVisitor<ACollidableFrustum>
{
public:
	ContainFrustumVisitor(const ACollidableFrustum* const collidableFrustum);
	~ContainFrustumVisitor() override = default;

public:
	virtual bool Visit(const ACollidableSphere* const collidableSphere) const override;
	virtual bool Visit(const ACollidableOrientedBox* const collidableOrientedBox) const override;
	virtual bool Visit(const ACollidableFrustum* const collidableFrustum) const override;
};
