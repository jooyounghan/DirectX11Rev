#pragma once
#include "IWorld.h"
#include "HeaderHelper.h"
#include "StaticAssertHelper.h"

#include "IUpdatable.h"
#include "Delegation.h"

#include "TaskAnalyzerWindow.h"
#include "ViewportWindow.h" 
#include "MapOutlinerWindow.h"
#include "AssetManagerWindow.h"

#include <windows.h>
#include <memory>
#include <unordered_map>
#include <vector>

class AWindow;
class AssetManager;
class MapAsset;

class GameWorld : public IWorld, public IUpdatable
{
public:
	GameWorld(HWND WindowHandle);
	virtual ~GameWorld();

private:
	void InitEditor(HWND WindowHandle);

public:
	virtual void Update(const float& DeltaTimeIn) override;

protected:
	UINT FontSrvHandleID = 0;
	MakeGetter(FontSrvHandleID);

protected:
	IDSelectHandler OnIDSelected;
	AssetDropHandler OnAssetDropped;

protected:
	std::vector<std::unique_ptr<AWindow>> Dialogs;

private:
	TaskAnalyzerWindow* TaskAnalyzerWindowInstance = nullptr;
	ViewportWindow* ViewportWindowInstance = nullptr;
	MapOutlinerWindow* MapOutlinerWindowInstance = nullptr;
	AssetManagerWindow* AssetManagerWindowInstance = nullptr;

private:
	template<typename T, typename... Args>
	T* AddDialog(Args... arg);

protected:
	std::shared_ptr<MapAsset> CurrentMap = nullptr;

private:
	MapSelectedHandler OnMapSelected;
	void SetCurrentMap(const std::shared_ptr<MapAsset>& NewMap);

public:
	virtual void Render() override;
	virtual void AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	virtual void ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	void OnDropFiles(HDROP hDropIn);
};

template<typename T, typename ...Args>
inline T* GameWorld::AddDialog(Args ...args)
{
	static_assert(std::is_base_of<AWindow, T>::value, DerivedCondition(AWindow));

	Dialogs.emplace_back(make_unique<T>(args...));
	return (T*)Dialogs.back().get();
}
