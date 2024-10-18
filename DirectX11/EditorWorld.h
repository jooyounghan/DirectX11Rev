#pragma once

#include "IWorld.h"
#include "HeaderHelper.h"

#include <windows.h>
#include <wrl/client.h>
#include <vector>
#include <memory>

class MapAsset;
class APlaceableObject;
class AAttachableObject;
class GameWorld;
class AWindow;

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
	std::vector<std::unique_ptr<AWindow>> Dialogs;

protected:
	MapAsset* SelectedMapAsset = nullptr;
	MakeGetter(SelectedMapAsset);

public:
	void SetSelectedMapAsset(MapAsset* MapAssetIn);

protected:
	APlaceableObject* SelectedPlaceable = nullptr;
	MakeGetter(SelectedPlaceable);

public:
	void SetSelectedPlaceable(APlaceableObject* APlaceableObjectIn);

protected:
	AAttachableObject* SelectedAttached = nullptr;
	MakeSetterGetter(SelectedAttached);

public:
	void SetSelecteObjectByID(const UINT& Id);

public:
	virtual void Render() override;

public:
	virtual void AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	virtual void ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	void OnDropFiles(HDROP hDropIn);
};

