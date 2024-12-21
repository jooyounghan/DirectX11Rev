#pragma once
#include "IAssetVisitor.h"

#include "AssetReader.h"
#include "AssetWriter.h"
#include "ResourceManager.h"

#include "BaseTextureAsset.h"
#include "ScratchTextureAsset.h"
#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"
#include "ModelMaterialAsset.h"
#include "IBLMaterialAsset.h"
#include "BoneAsset.h"
#include "AnimationAsset.h"
#include "MapAsset.h"

class AssetManager : public IBaseTextureProvider, public IScratchTextureProvider, public IStaticMeshProvider,
	public ISkeletalMeshProvider, public IModelMaterialProvider, public IIBLMaterialProvider, public IBoneProvider,
	public IAnimationProvider, public IMapProvider, public IResourceProvider
{
public:
	AssetManager();
	~AssetManager();

protected:
	std::unordered_map<std::string, AssetReader> m_assetReadersWithPath;
	std::unordered_map<std::string, std::vector<AAssetWriter*>> m_assetWritersWithPath;
	ResourceManager m_resourceManager;

protected:
	std::unordered_map<EAssetType, std::unordered_map<std::string, AAsset*>> m_assetNameToAssets;
	
public:
	void RegisterAssetReadPath(const std::string& readPath);
	void RegisterAssetWritePath(const std::string& writePath);

public:
	void PreloadFromResources();
	void PreloadFromDirectories(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void WrtieFileAsAsset(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string filePath);

private:
	void AddAssetHelper(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const EAssetType& assetType, std::string assetPath, AAsset* asset);

private:
	std::unordered_map<
		std::string, 
		std::function<void(const EAssetType&, std::string, AAsset*)>
	> OnAssetLoaded;
	
public:
	void RegisterAssetLoadedHandler(const std::string& handlerName, const std::function<void(const EAssetType&, std::string, AAsset*)>& handler);
	void DeregisterAssetLoadedHandler(const std::string& handlerName);

private:
	void InvokeAssetLoadedHandler(const EAssetType& assetType, std::string assetPath, AAsset* asset);

public:
	virtual BaseTextureAsset* const GetBaseTextureAsset(const std::string& assetName) override;
	virtual ScratchTextureAsset* const GetScratchTextureAsset(const std::string& assetName) override;
	virtual StaticMeshAsset* const GetStaticMeshAsset(const std::string& assetName) override;
	virtual SkeletalMeshAsset* const GetSkeletalMeshAsset(const std::string& assetName) override;
	virtual ModelMaterialAsset* const GetModelMaterialAsset(const std::string& assetName) override;
	virtual IBLMaterialAsset* const GetIBLMaterialAsset(const std::string& assetName) override;
	virtual BoneAsset* const GetBoneAsset(const std::string& assetName) override;
	virtual AnimationAsset* const GetAnimationAsset(const std::string& assetName) override;
	virtual MapAsset* const GetMapAsset(const std::string& assetName) override;
	virtual BaseTextureAsset* const GetResourceAsset(const std::string& assetName) override;

private:
	template<typename T>
	T* const GetAssetHelper(const EAssetType& asssetType, const std::string& assetName);
};
