#pragma once

#include "IWorld.h"
#include "HeaderHelper.h"

#include <windows.h>
#include <wrl/client.h>
#include <vector>
#include <memory>

class AObject;
class PlaceableObject;
class GameWorld;
class IWindow;
class EditorActor;

class EditorWorld : public IWorld
{
public:
	EditorWorld(GameWorld* GameWorldIn, HWND WindowHandle);
	~EditorWorld();

protected:
	GameWorld* GameWorldCached = nullptr;
	MakeGetter(GameWorldCached);

protected:
	UINT FontSrvHandleID = 0;
	MakeGetter(FontSrvHandleID);

protected:
	std::unique_ptr<EditorActor> EditorActorInstance;
	MakeSmartPtrGetter(EditorActorInstance);

protected:
	std::vector<std::unique_ptr<IWindow>> Dialogs;

protected:
	PlaceableObject* SelectedPlaceable = nullptr;
	MakeSetterGetter(SelectedPlaceable);

protected:
	AObject* SelectedDetailed = nullptr;
	MakeSetterGetter(SelectedDetailed);

public:
	void SetSelecteObjectByID(const UINT& Id);

public:
	virtual void UpdateWorld(const float& DeltaTimeIn) override;
	virtual void RenderWorld() override;

public:
	virtual void AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	virtual void ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	void OnDropFiles(HDROP hDropIn);
};

