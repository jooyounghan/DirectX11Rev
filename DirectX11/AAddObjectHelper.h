#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

class MapAsset;
class EditorWorld;
class GraphicsPipeline;

class AAddObjectHelper
{
public:
	AAddObjectHelper(EditorWorld* EditorWorldIn, MapAsset* CurrentMap);

protected:
	EditorWorld* EditorWorldCached = nullptr;
	MapAsset* CurrentMapCached = nullptr;

public:
	virtual void DrawAddObjectPopup() = 0;
};

