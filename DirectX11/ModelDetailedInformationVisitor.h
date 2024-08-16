#pragma once
#include "IGuiModelVisitor.h"
#include <DirectXMath.h>

class ModelDetailWindow;


class ModelDetailedInformationVisitor : public IGuiModelVisitor
{
public:
	ModelDetailedInformationVisitor(ModelDetailWindow* ModelDetailWindowIn);

protected:
	ModelDetailWindow* ModelDetailWindowCached;

public:
	virtual void Visit(MeshObject* AttachableInstance) override;

public:
	virtual void Visit(BoundingSphere* BoundingSphereInstance) override;
	virtual void Visit(OrientedBoundingBox* OBBInstance) override;
	virtual void Visit(BoundingFrustum* BoundingFrustumInstance) override;

public:
	virtual void Visit(Viewable* BoundingFrustumInstance) override;

};



