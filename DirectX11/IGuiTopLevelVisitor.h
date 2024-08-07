#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

class AttachableObject;
class PlaceableObject;
class RelativePlaceableObject;

class IGuiTopLevelVisitor
{
public:
	virtual void Visit(AttachableObject* AttachableInstance) = 0;
	virtual void Visit(PlaceableObject* PlaceableInstance) = 0;
	virtual void Visit(RelativePlaceableObject* RelativePlaceableObject) = 0;
};