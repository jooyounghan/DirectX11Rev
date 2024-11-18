#pragma once
#include "IWorld.h"

#include "HeaderHelper.h"
#include "StaticAssertHelper.h"
#include "Delegation.h"

#include "TaskAnalyzerWindow.h"
#include "MapOutlinerWindow.h"
#include "AssetManagerWindow.h"

#include <memory>
#include <vector>

class AWindow;
class AssetManager;
class MapAsset;

class GameWorld : public IWorld
{
public:
	GameWorld(HWND WindowHandle);
	virtual ~GameWorld();

protected:
	UINT FontSrvHandleID = 0;
	MakeGetter(FontSrvHandleID);

protected:
	IDSelectHandler OnIDSelected;
	AssetDropHandler OnAssetDropped;
	MapSelectedHandler OnMapSelected;

protected:
	std::vector<std::unique_ptr<AWindow>> Dialogs;

private:
	TaskAnalyzerWindow* TaskAnalyzerWindowInstance = nullptr;
	MapOutlinerWindow* MapOutlinerWindowInstance = nullptr;
	AssetManagerWindow* AssetManagerWindowInstance = nullptr;

private:
	template<typename T, typename... Args>
	T* AddDialog(Args... arg);

private:
	void InitEditor(HWND WindowHandle);
	void SetCurrentMap(const std::shared_ptr<MapAsset>& NewMap);
	void SelectPlaceableByID(const UINT& SelectedID);
	void AddAssetWithDropped(AAssetFile* AssetFileIn, const float& PosXIn, const float& PosYIn, const float& PosZIn);

protected:
	std::shared_ptr<MapAsset> CurrentMap = nullptr;

public:
	void Update(const float& DeltaTimeIn);
	void Render();
	virtual void AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	void ResolveGBuffer();
	void RenderUI();

private:
	virtual void ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	void OnDropFiles(HDROP hDropIn);
};
