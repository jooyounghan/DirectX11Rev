#pragma once
#include "AWindow.h"
#include "AssetManager.h"
#include "FileControl.h"

struct SAssetFolder
{
	std::vector<FileControl> assetControls;
	std::unordered_map<std::string, SAssetFolder> childFolders;
};

class AssetViewWindow : public AWindow
{
public:
	AssetViewWindow(const std::string& windowID, AssetManager* assetManager);

private:
	AssetManager* m_assetManagerCached = nullptr;
	SAssetFolder  m_assetRootFolders;
	const SAssetFolder* m_selectedFolders = nullptr;

public:
	virtual void RenderWindowImpl() override;

private:
	void AddAssetControl(const std::string& assetPath, AAsset* asset);
	void AddAssetByRecursiveKey(
		SAssetFolder& targetFolder, const std::string& assetPath, AAsset* asset
	);

private:
	void RenderAssetFolderStructureByRecursive(
		const std::string& folderName, 
		const SAssetFolder* const assetFolder
	);
};

