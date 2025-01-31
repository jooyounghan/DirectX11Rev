#pragma once
#include "CollisionVisitor.h"

class ContainVisitor : public ACollisionVisitor
{
public:
	ContainVisitor(ICollisionAcceptor* collisionAcceptor);
	~ContainVisitor() override = default;

public:
	virtual bool Visit(const CollidableSphere* const collidableSphere) const override;
	virtual bool Visit(const CollidableOrientedBox* const collidableOrientedBox) const override;
	virtual bool Visit(const CollidableFrustum* const collidableFrustum) const override;
};


class ContainSphereVisitor : public ACollisionSpecifiedVisitor<CollidableSphere>
{
public:
	ContainSphereVisitor(const CollidableSphere* const collidableSphere);
	~ContainSphereVisitor() override = default;

public:
	virtual bool Visit(const CollidableSphere* const collidableSphere) const override;
	virtual bool Visit(const CollidableOrientedBox* const collidableOrientedBox) const override;
	virtual bool Visit(const CollidableFrustum* const collidableFrustum) const override;
};

class ContainOrientedBoxVisitor : public ACollisionSpecifiedVisitor<CollidableOrientedBox>
{
public:
	ContainOrientedBoxVisitor(const CollidableOrientedBox* const collidableOrientedBox);
	~ContainOrientedBoxVisitor() override = default;


public:
	virtual bool Visit(const CollidableSphere* const collidableSphere) const override;
	virtual bool Visit(const CollidableOrientedBox* const collidableOrientedBox) const override;
	virtual bool Visit(const CollidableFrustum* const collidableFrustum) const override;
};


class ContainFrustumVisitor : public ACollisionSpecifiedVisitor<CollidableFrustum>
{
public:
	ContainFrustumVisitor(const CollidableFrustum* const collidableFrustum);
	~ContainFrustumVisitor() override = default;

public:
	virtual bool Visit(const CollidableSphere* const collidableSphere) const override;
	virtual bool Visit(const CollidableOrientedBox* const collidableOrientedBox) const override;
	virtual bool Visit(const CollidableFrustum* const collidableFrustum) const override;
};
