#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

class MeshObject;

class BoundingSphere;
class OrientedBoundingBox;
class BoundingFrustum;

class Viewable;

class IGuiLowLevelVisitor
{
public:
	virtual void Visit(MeshObject* MeshObjectInstance) = 0;

public:
	virtual void Visit(BoundingSphere* BoundingSphereInstance) = 0;
	virtual void Visit(OrientedBoundingBox* OBBInstance) = 0;
	virtual void Visit(BoundingFrustum* BoundingFrustumInstance) = 0;

public:
	virtual void Visit(Viewable* BoundingFrustumInstance) = 0;
};