#pragma once
#include "ICollisionVisitor.h"

class IIntersectable;

class CollisionVisitor : public ICollisionVisitor
{
public:
	CollisionVisitor(IIntersectable* CheckObjectIn);

protected:
	IIntersectable* CheckObjectCahced;

public:
	virtual bool Visit(BoundingSphereObject* BSInstance) override;
	virtual bool Visit(OBBObject* OBBInstance) override;
	virtual bool Visit(BoundingFrustumObject* FrustumInstance) override;

protected:
	template<typename T>
	class CollisionVisitorWrapper : public ICollisionVisitor
	{
	public:
		CollisionVisitorWrapper(T* SpecifiedObject);

	protected:
		T* SpecifiedObjectCached;

	public:
		virtual bool Visit(BoundingSphereObject* BSInstance) override;
		virtual bool Visit(OBBObject* OBBInstance) override;
		virtual bool Visit(BoundingFrustumObject* FrustumInstance) override;
	};
};

class CollisionLogic
{
public:
	static bool CheckCollision(BoundingSphereObject* t1, BoundingSphereObject* t2);
	static bool CheckCollision(BoundingSphereObject* t1, OBBObject* t2);
	static bool CheckCollision(BoundingSphereObject* t1, BoundingFrustumObject* t2);
	static bool CheckCollision(OBBObject* t1, BoundingSphereObject* t2);
	static bool CheckCollision(OBBObject* t1, OBBObject* t2);
	static bool CheckCollision(OBBObject* t1, BoundingFrustumObject* t2);
	static bool CheckCollision(BoundingFrustumObject* t1, BoundingSphereObject* t2);
	static bool CheckCollision(BoundingFrustumObject* t1, OBBObject* t2);
	static bool CheckCollision(BoundingFrustumObject* t1, BoundingFrustumObject* t2);
};

template<typename T>
inline CollisionVisitor::CollisionVisitorWrapper<T>::CollisionVisitorWrapper(T* SpecifiedObject)
	: SpecifiedObjectCached(SpecifiedObject)
{
}

template<typename T>
inline bool CollisionVisitor::CollisionVisitorWrapper<T>::Visit(BoundingSphereObject* BSInstance)
{
	return CollisionLogic::CheckCollision(SpecifiedObjectCached, BSInstance);
}

template<typename T>
inline bool CollisionVisitor::CollisionVisitorWrapper<T>::Visit(OBBObject* OBBInstance)
{
	return CollisionLogic::CheckCollision(SpecifiedObjectCached, OBBInstance);
}

template<typename T>
inline bool CollisionVisitor::CollisionVisitorWrapper<T>::Visit(BoundingFrustumObject* FrustumInstance)
{
	return CollisionLogic::CheckCollision(SpecifiedObjectCached, FrustumInstance);
}
