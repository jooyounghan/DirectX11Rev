#pragma once
#include "AWindow.h"
#include <memory>

enum class EAssetThumbnailType
{
	ASSET_THUMBNAIL_TYPE_ANIMATION,
	ASSET_THUMBNAIL_TYPE_BONE,
	ASSET_THUMBNAIL_TYPE_TEXTURE,
	ASSET_THUMBNAIL_TYPE_STATIC,
	ASSET_THUMBNAIL_TYPE_SKELETAL,
	ASSET_THUMBNAIL_TYPE_MATERIAL,
};

constexpr size_t AssetThumbnailTypeCount = static_cast<size_t>(EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_MATERIAL) + 1;

class AAsset;
class AssetFileControl;
enum class EAssetType;

class AssetContextMenu;

struct ID3D11Device;
struct ID3D11DeviceContext;

struct SAssetFolder
{
	std::vector<std::unique_ptr<AssetFileControl>> m_assetControls;
	std::unordered_map<std::string, SAssetFolder> m_childFolders;
};


class AssetViewWindow : public AWindow
{
public:
	AssetViewWindow(
		const std::string& windowID, 
		ID3D11Device* const* deviceAddress,
		ID3D11DeviceContext* const* deviceContextAddress
	);
	~AssetViewWindow() override;

private:
	SAssetFolder  m_assetRootFolders;
	const SAssetFolder* m_selectedFolders = nullptr;

private:
	AssetContextMenu* m_assetContextMenuModal = nullptr;
	std::vector<AWindow*> m_assetEditorWindows;

private:
	static std::string AnimationAssetTN;
	static std::string BoneAssetTN;
	static std::string TextureAssetTN;
	static std::string SkeletalMeshAssetTN;
	static std::string StaticMeshAssetTN;
	static std::string MapAssetTN;
	static std::string MaterialAssetTN;

public:
	virtual void PrepareWindow() override;

private:
	virtual void RenderWindowImpl() override;

private:
	void AddAssetControl(const EAssetType& assetType, const std::string& assetPath, AAsset* asset);
	void AddAssetByRecursiveKey(
		const EAssetType& assetType, SAssetFolder& targetFolder, const std::string& assetPath, AAsset* asset
	);

private:
	void RenderAssetFolderStructureByRecursive(
		const std::string& folderName, 
		const SAssetFolder* const assetFolder
	);
	void RenderSelectedAssetFolders(const SAssetFolder* const assetFolder);
};

