#pragma once

#include "IWorld.h"
#include "HeaderHelper.h"

#include <windows.h>
#include <wrl/client.h>
#include <vector>
#include <memory>

class APlaceableObject;
class AAttachableObject;
class GameWorld;
class IWindow;

class EditorWorld : public IWorld
{
public:
	EditorWorld(GameWorld* GameWorldIn, HWND WindowHandle);
	virtual ~EditorWorld();

protected:
	GameWorld* GameWorldCached = nullptr;
	MakeGetter(GameWorldCached);

protected:
	UINT FontSrvHandleID = 0;
	MakeGetter(FontSrvHandleID);

protected:
	std::vector<std::unique_ptr<IWindow>> Dialogs;

protected:
	APlaceableObject* SelectedPlaceable = nullptr;
	MakeSetterGetter(SelectedPlaceable);

protected:
	AAttachableObject* SelectedAttached = nullptr;
	MakeSetterGetter(SelectedAttached);

public:
	void SetSelecteObjectByID(const UINT& Id);

public:
	virtual void RenderWorld() override;

public:
	virtual void AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	virtual void ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	void OnDropFiles(HDROP hDropIn);
};

