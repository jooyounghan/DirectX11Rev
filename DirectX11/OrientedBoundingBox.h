#pragma once
#include "IIntersectable.h"
#include "AttachableObject.h"

class PlacableObject;
//
//class OrientedBoundingBox : public IIntersectable, public AttachableObject
//{
//public:
//	OrientedBoundingBox() {};
//	virtual ~OrientedBoundingBox() {};
//
//protected:
//	SPosition3D			Center;
//	SVector3D			Extents;
//	DirectX::XMMATRIX	Orientation;
//
//
//public:
//	virtual bool Intersect(const Ray& RayIn, float& DistanceOut) override;
//	virtual void UpdateObject(const float& DeltaTimeIn, IObject* ParentObject) override;
//};

