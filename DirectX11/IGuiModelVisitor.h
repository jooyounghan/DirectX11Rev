#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

class MeshObjectActor;
class EnvironmentActor;
class EditorPawn;

class MeshObject;
class BoundingSphereObject;
class OBBObject;
class BoundingFrustumObject;
class Viewable;


class IGuiModelVisitor
{
public:
	virtual void Visit(MeshObjectActor* MeshObjectActorInstance) = 0;
	virtual void Visit(EnvironmentActor* EnvironmentActorInstance) = 0;
	virtual void Visit(EditorPawn* EditorActorInstance) = 0;

public:
	virtual void Visit(MeshObject* MeshObjectInstance) = 0;
	virtual void Visit(BoundingSphereObject* BoundingSphereInstance) = 0;
	virtual void Visit(OBBObject* OBBInstance) = 0;
	virtual void Visit(BoundingFrustumObject* BoundingFrustumInstance) = 0;
	virtual void Visit(Viewable* BoundingFrustumInstance) = 0;
};