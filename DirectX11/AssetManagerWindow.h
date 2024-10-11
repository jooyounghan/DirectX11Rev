#pragma once
#include "IWindow.h"
#include "AssetManager.h"
#include "AssetControl.h"
#include "AssetControlWindow.h"

struct DirectorySet
{
	std::string Name;
	std::string Directory;
	std::list<DirectorySet> ChildrenDirectories;
	std::list<AssetControl> AssetControls;
};

class AssetControlWindow;

class AssetManagerWindow : public IWindow
{
public:
	AssetManagerWindow(AssetManager* AssetManagerIn);
	virtual ~AssetManagerWindow();

private:
	AssetManager* AssetManagerCached = nullptr;

public:
	virtual void RenderWindow() override;


private:
	DirectorySet RootDirectory;

private:
	DirectorySet* SelectedDirectory = nullptr;
	AssetControl* SelectedAssetControl = nullptr;

protected:
	AssetAddedDelegate OnAssetChanged;

protected:
	AssetControlBeginDragDropDelegate OnAssetControlBeginDragDrop;
	
protected:
	AssetControlStyleChangeDelegation OnAssetControlPushHilightStyle;
	AssetControlStyleChangeDelegation OnAssetControlPopHilightStyle;

protected:
	AssetControlClickedDelegate OnAssetLeftMouseClicked;
	AssetControlClickedDelegate OnAssetLeftMouseDBClicked;

protected:
	AssetControlWindowClosedDelegation OnAssetControlWindowClosed;

protected:
	std::list<AssetControlWindow> AssetControlWindows;

private:
	void RenderAssetDirectories();
	void RenderAssetDirectoriesHelper(DirectorySet& DirectorySetIn);

private:
	void RenderAssetControls();

private:
	void RenderAssetControlWindows();

private:
	void RefreshAssetDirectoriesFromRoot();
	void BuildAssetDirectories(DirectorySet& DirectorySetIn);

private:
	void SetAssetControlDragDrop(AAssetFile* AssetFileCached);

private:
	void HilightItem(AssetControl* AssetControlCached);
	void UnhilightItem(AssetControl* AssetControlCached);

private:
	void FocusItem(AssetControl* AssetControlCached);
	void OpenItemSetting(AssetControl* AssetControlCached);

private:
	std::vector<AssetControlWindow*> CloseAssetControlWindowsList;
	void AddCloseAssetControlWindowList(AssetControlWindow* AssetControlWindowIn);
	void CloseAssetControlWindow();
};

