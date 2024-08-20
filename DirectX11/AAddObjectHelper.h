#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

class Map;
class EditorWorld;
class GraphicsPipeline;

class AAddObjectHelper
{
public:
	AAddObjectHelper(EditorWorld* EditorWorldIn, Map* CurrentMap);

protected:
	EditorWorld* EditorWorldCached = nullptr;
	Map* CurrentMapCached = nullptr;

public:
	virtual void DrawAddObjectPopup() = 0;
};

