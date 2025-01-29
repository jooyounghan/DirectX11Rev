#pragma once
#include "CollidableSphere.h"
#include "CollidableOrientedBox.h"
#include "CollidableFrustum.h"

class ICollisionVisitor
{
public:
	virtual ~ICollisionVisitor() = default;

public:
	virtual bool Visit(const CollidableSphere* const collidableSphere) const = 0;
	virtual bool Visit(const CollidableOrientedBox* const collidableSphere) const = 0;
	virtual bool Visit(const CollidableFrustum* const collidableSphere) const = 0;
};

class ACollisionVisitor : public ICollisionVisitor
{
public:
	ACollisionVisitor(ICollisionAcceptor* collisionAcceptor);
	~ACollisionVisitor() override = default;

protected:
	ICollisionAcceptor* m_collisionAcceptor;
};

template<typename T>
class ACollisionSpecifiedVisitor : public ICollisionVisitor
{
public:
	ACollisionSpecifiedVisitor(const T* const specifiedCollisionAcceptor);
	~ACollisionSpecifiedVisitor() override = default;

public:
	const T* const  m_specifiedCollisionAccpetor;
};

template<typename T>
ACollisionSpecifiedVisitor<T>::ACollisionSpecifiedVisitor(const T* const specifiedCollisionAcceptor)
	: m_specifiedCollisionAccpetor(specifiedCollisionAcceptor)
{
}
