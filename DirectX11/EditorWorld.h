#pragma once

#include "IWorld.h"
#include "HeaderHelper.h"

#include <windows.h>
#include <wrl/client.h>
#include <vector>
#include <memory>

class GameWorld;
class IWindow;
class EditorCamera;

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
	std::unique_ptr<EditorCamera> EditorCameraInstance;
	MakeGetter(EditorCameraInstance);

protected:
	std::vector<std::unique_ptr<IWindow>> Dialogs;

public:
	virtual void UpdateWorld(const float& DeltaTimeIn) override;
	virtual void RenderWorld() override;

public:
	virtual void AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	virtual void ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	void OnDropFiles(HDROP hDropIn);
};

