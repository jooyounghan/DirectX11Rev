#pragma once

class IIntersectable;

class BoundingSphere;
class OrientedBoundingBox;
class BoundingFrustum;

class ICollisionVisitor
{
public:
	virtual bool Visit(BoundingSphere* BSInstance) = 0;
	virtual bool Visit(OrientedBoundingBox* OBBInstance) = 0;
	virtual bool Visit(BoundingFrustum* FrustumInstance) = 0;
};

class CollisionVisitor : public ICollisionVisitor
{
public:
	CollisionVisitor(IIntersectable* CheckObjectIn);

protected:
	IIntersectable* CheckObjectCahced;

public:
	virtual bool Visit(BoundingSphere* BSInstance) override;
	virtual bool Visit(OrientedBoundingBox* OBBInstance) override;
	virtual bool Visit(BoundingFrustum* FrustumInstance) override;

protected:
	template<typename T>
	class CollisionVisitorWrapper : public ICollisionVisitor
	{
	public:
		CollisionVisitorWrapper(T* SpecifiedObject);

	protected:
		T* SpecifiedObjectCached;

	public:
		virtual bool Visit(BoundingSphere* BSInstance) override;
		virtual bool Visit(OrientedBoundingBox* OBBInstance) override;
		virtual bool Visit(BoundingFrustum* FrustumInstance) override;
	};
};

class CollisionLogic
{
public:
	static bool CheckCollision(BoundingSphere* t1, BoundingSphere* t2);
	static bool CheckCollision(BoundingSphere* t1, OrientedBoundingBox* t2);
	static bool CheckCollision(BoundingSphere* t1, BoundingFrustum* t2);
	static bool CheckCollision(OrientedBoundingBox* t1, BoundingSphere* t2);
	static bool CheckCollision(OrientedBoundingBox* t1, OrientedBoundingBox* t2);
	static bool CheckCollision(OrientedBoundingBox* t1, BoundingFrustum* t2);
	static bool CheckCollision(BoundingFrustum* t1, BoundingSphere* t2);
	static bool CheckCollision(BoundingFrustum* t1, OrientedBoundingBox* t2);
	static bool CheckCollision(BoundingFrustum* t1, BoundingFrustum* t2);
};

template<typename T>
inline CollisionVisitor::CollisionVisitorWrapper<T>::CollisionVisitorWrapper(T* SpecifiedObject)
	: SpecifiedObjectCached(SpecifiedObject)
{
}

template<typename T>
inline bool CollisionVisitor::CollisionVisitorWrapper<T>::Visit(BoundingSphere* BSInstance)
{
	return CollisionLogic::CheckCollision(SpecifiedObjectCached, BSInstance);
}

template<typename T>
inline bool CollisionVisitor::CollisionVisitorWrapper<T>::Visit(OrientedBoundingBox* OBBInstance)
{
	return CollisionLogic::CheckCollision(SpecifiedObjectCached, OBBInstance);
}

template<typename T>
inline bool CollisionVisitor::CollisionVisitorWrapper<T>::Visit(BoundingFrustum* FrustumInstance)
{
	return CollisionLogic::CheckCollision(SpecifiedObjectCached, FrustumInstance);
}
