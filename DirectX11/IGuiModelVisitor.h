#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

class Actor;
class EditorActor;

class MeshObject;
class BoundingSphereObject;
class OBBObject;
class BoundingFrustumObject;
class Viewable;


class IGuiModelVisitor
{
public:
	virtual void Visit(Actor* ActorInstance) = 0;
	virtual void Visit(EditorActor* EditorActorInstance) = 0;

public:
	virtual void Visit(MeshObject* MeshObjectInstance) = 0;
	virtual void Visit(BoundingSphereObject* BoundingSphereInstance) = 0;
	virtual void Visit(OBBObject* OBBInstance) = 0;
	virtual void Visit(BoundingFrustumObject* BoundingFrustumInstance) = 0;
	virtual void Visit(Viewable* BoundingFrustumInstance) = 0;
};