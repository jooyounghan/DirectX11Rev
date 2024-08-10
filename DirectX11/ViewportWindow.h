#pragma once
#include "IGameWorldLinkedWindow.h"

class Map;
class EditorCamera; 

class ViewportWindow : public IGameWorldLinkedWindow
{
public: 
	ViewportWindow(GameWorld* GameWorldLinkedIn, EditorCamera* EditorCameraIn);
	virtual ~ViewportWindow();

protected:
	EditorCamera* EditorCameraCached = nullptr;

public:
	virtual void RenderWindow() override;

private:
	static std::string ViewportWindowName;

private:
	void ManageAssetDrop(Map* CurrentMap);
	void ManageMouseLBClick(Map* CurrentMap);
};

