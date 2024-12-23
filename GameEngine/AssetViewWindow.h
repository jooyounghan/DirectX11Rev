#pragma once
#include "AWindow.h"
#include "AssetManager.h"
#include "AssetFileControl.h"
#include "Texture2DInstance.h"
#include "SRVOption.h"

enum class EAssetThumbnailType
{
	ASSET_THUMBNAIL_TYPE_ANIMATION,
	ASSET_THUMBNAIL_TYPE_BONE,
	ASSET_THUMBNAIL_TYPE_TEXTURE,
	ASSET_THUMBNAIL_TYPE_STATIC,
	ASSET_THUMBNAIL_TYPE_SKELETAL,
	ASSET_THUMBNAIL_TYPE_MATERIAL,
	ASSET_THUMBNAIL_TYPE_MAP
};

constexpr size_t AssetThumbnailTypeCount = static_cast<size_t>(EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_MAP) + 1;

struct SAssetFolder
{
	std::vector<std::unique_ptr<AssetFileControl>> m_assetControls;
	std::unordered_map<std::string, SAssetFolder> m_childFolders;
};

class AssetViewWindow : public AWindow
{
public:
	AssetViewWindow(const std::string& windowID, AssetManager* assetManager);

private:
	AssetManager* m_assetManagerCached = nullptr;
	SAssetFolder  m_assetRootFolders;
	const SAssetFolder* m_selectedFolders = nullptr;

private:
	static std::string AnimationAssetTN;
	static std::string BoneAssetTN;
	static std::string TextureAssetTN;
	static std::string SkeletalMeshAssetTN;
	static std::string StaticMeshAssetTN;
	static std::string MapAssetTN;
	static std::string MaterialAssetTN;

private:
	std::unique_ptr<Texture2DInstance<SRVOption>> m_thumbnails[AssetThumbnailTypeCount];
	void InitThumbnailTextureAsAsset(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const EAssetThumbnailType& assetType, const UINT& widht, const UINT& height, BaseTextureAsset* const asset);

public:
	virtual void RenderWindowImpl() override;
	virtual void InitializeWindow(ID3D11Device* device, ID3D11DeviceContext* deviceContext) override;

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

