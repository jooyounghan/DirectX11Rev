#pragma once

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
