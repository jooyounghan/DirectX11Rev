#pragma once
#include "IWindow.h"
#include "AssetManager.h"

#include <vector>
#include <filesystem>

struct DirectorySet
{
	std::string Name;
	std::string Directory;
	std::vector<DirectorySet> ChildrenDirectories;
};

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
	void RenderAssetDirectoryStructure();

private:
	DirectorySet RootDirectory;
	DirectorySet* SelectedDirectory = nullptr;
	void RenderAssetDirectories(DirectorySet& DirectorySetIn);

protected:
	AssetAddedDelegate OnAssetAdded;

private:
	void RenderCurrentDirectoryAsset();
	void RenderAssetFile(const std::filesystem::path& AssetPathIn, const float& VisibleWidthIn);

private:
	void RefreshAssetDirectories();
	void TravelAssetDirectories(DirectorySet& DirectorySetIn);
};

