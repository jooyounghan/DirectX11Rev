#pragma once
#include "IGuiLowLevelVisitor.h"
#include <DirectXMath.h>

class AttachableObject;
class PlaceableObject;
class RelativePlaceableObject;

class MapOutlinerWindow;


class ModelInformationOutlinerVisitor : public IGuiLowLevelVisitor
{
public:
	ModelInformationOutlinerVisitor(MapOutlinerWindow* MapOutlinerInstance);

protected:
	MapOutlinerWindow* MapOutlinerCached;

public:
	virtual void Visit(MeshObject* AttachableInstance) override;

public:
	virtual void Visit(BoundingSphere* BoundingSphereInstance) override;
	virtual void Visit(OrientedBoundingBox* OBBInstance) override;
	virtual void Visit(BoundingFrustum* BoundingFrustumInstance) override;

public:
	virtual void Visit(Viewable* BoundingFrustumInstance) override;

};



