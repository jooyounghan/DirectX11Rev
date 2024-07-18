#pragma once

#include "IWorld.h"
#include "HeaderHelper.h"

#include <windows.h>
#include <wrl/client.h>
#include <vector>
#include <memory>

class GameWorld;
class IDialog;
class Camera;

class EditorWorld : public IWorld
{
public:
	EditorWorld(GameWorld* GameWorldIn, HWND WindowHandle);
	~EditorWorld();

protected:
	GameWorld* GameWorldCached = nullptr;

protected:
	UINT FontSrvHandleID = 0;
	MakeGetter(FontSrvHandleID);

protected:
	std::vector<std::unique_ptr<IDialog>> Dialogs;

protected:
	std::unique_ptr<Camera> EditorCamera;
	MakeSmartPtrGetter(EditorCamera);

public:
	virtual void RenderWorld() override;

public:
	virtual LRESULT AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

