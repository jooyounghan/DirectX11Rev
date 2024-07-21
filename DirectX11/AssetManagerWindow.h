#pragma once
#include "IWindow.h"
#include <vector>
#include <string>
#include <filesystem>

class AssetManager;

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
	const ImGuiTreeNodeFlags BaseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

public:
	virtual void RenderWindow() override;

private:
	void RenderAssetDirectoryStructure();

private:
	DirectorySet RootDirectory;
	DirectorySet* SelectedDirectory = nullptr;
	void TravelAssetDirectories(DirectorySet& DirectorySetIn);
	void RenderAssetDirectories(DirectorySet& DirectorySetIn);

public:
	void RefreshAssetDirectories();

private:
	static ImVec2 FileSize;
	void RenderCurrentDirectoryAsset();
	void RenderAssetFile(const std::filesystem::path& AssetPathIn, const float& VisibleWidthIn);
};

