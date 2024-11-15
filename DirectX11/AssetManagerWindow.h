#pragma once
#include "AWindow.h"
#include "AssetManager.h"
#include "AssetControl.h"

#include "AddAssetFilePopup.h"

struct DirectorySet
{
	std::string Name;
	std::string Directory;
	std::list<DirectorySet> ChildrenDirectories;
	std::list<AssetControl> AssetControls;
};

class CreateAssetModal;

class AssetManagerWindow : public AWindow
{
public:
	AssetManagerWindow();
	virtual ~AssetManagerWindow();

public:
	virtual void RenderWindow() override;

private:
	AddAssetFilePopup AddAssetFilePopupInstance;

private:
	std::unordered_map<std::string, std::unique_ptr<CreateAssetModal>> CreateAssetModalInstances;

private:
	DirectorySet RootDirectory;

private:
	DirectorySet* SelectedDirectory = nullptr;
	AssetControl* SelectedAssetControl = nullptr;

protected:
	AssetAddedHandler OnAssetChanged;

protected:
	AssetControlBeginDragDropHandler OnAssetControlBeginDragDrop;
	
protected:
	AssetControlStyleChangeHandler OnAssetControlPushHilightStyle;
	AssetControlStyleChangeHandler OnAssetControlPopHilightStyle;

protected:
	AssetControlClickedHandler OnAssetLeftMouseClicked;
	AssetControlClickedHandler OnAssetLeftMouseDBClicked;

protected:
	WindowClosedHandler OnAssetControlWindowClosed;

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
	AssetCreatedHandler OnCreateMaterialAsset;
	AssetCreatedHandler OnCreateRetargetedAnimationAsset;
	void StartCreateMaterialAsset();
	void StartCreateRetargetedAnimationAsset();
};

