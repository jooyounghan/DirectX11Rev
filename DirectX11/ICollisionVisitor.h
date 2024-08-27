#pragma once

class BoundingSphereObject;
class OBBObject;
class BoundingFrustumObject;

class ICollisionVisitor
{
public:
	virtual bool Visit(BoundingSphereObject* BSInstance) = 0;
	virtual bool Visit(OBBObject* OBBInstance) = 0;
	virtual bool Visit(BoundingFrustumObject* FrustumInstance) = 0;
};
