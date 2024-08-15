#pragma once
#include "IEditorLinkedWindow.h"

class Map;
class EditorCamera; 

class ViewportWindow : public IEditorLinkedWindow
{
public: 
	ViewportWindow(EditorWorld* EditorWorldIn);
	virtual ~ViewportWindow();

public:
	virtual void RenderWindow() override;

private:
	EditorCamera* EditorCameraCached = nullptr;

private:
	ImVec2 ImagePosition = ImVec2();
	ImVec2 ImageSize = ImVec2();

private:
	void ManageAssetDrop(Map* CurrentMap);
	void ManageMouseLBClick(Map* CurrentMap);
};

