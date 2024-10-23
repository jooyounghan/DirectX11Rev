#pragma once
#include "AWindow.h"
#include "AssetManager.h"
#include "AssetControl.h"

#include "AddAssetFilePopup.h"
#include "CreateAssetFileModal.h"

struct DirectorySet
{
	std::string Name;
	std::string Directory;
	std::list<DirectorySet> ChildrenDirectories;
	std::list<AssetControl> AssetControls;
};

class AssetManagerWindow : public AWindow
{
public:
	AssetManagerWindow(AssetManager* AssetManagerIn);
	virtual ~AssetManagerWindow();

private:
	AssetManager* AssetManagerCached = nullptr;

public:
	virtual void RenderWindow() override;

private:
	AddAssetFilePopup AddAssetFilePopupInstance;
	CreateAssetFileModal CreateAssetFileModalInstance;

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
	WindowClosedDelegation OnAssetControlWindowClosed;

protected:
	std::list<std::unique_ptr<AWindow>> AssetControlWindows;

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
	void StartHilightItem(AssetControl* AssetControlCached);
	void EndHilightItem(AssetControl* AssetControlCached);

private:
	void FocusItem(AssetControl* AssetControlCached);
	void OpenItemSetting(AssetControl* AssetControlCached);

private:
	std::vector<AWindow*> CloseAssetControlWindowsList;
	void AddCloseAssetControlWindowList(AWindow* AssetControlWindowIn);
	void CloseAssetControlWindow();

private:
	AssetCreatedHandler OnCreateAsset;
	void StartCreateAsset();
};

