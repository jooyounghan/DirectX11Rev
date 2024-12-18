#pragma once
#include "AssetWrapper.h"
#include <memory>
#include <unordered_map>
#include <set>

class AAssetWriter
{
public:
	AAssetWriter(const std::string& assetSavePath);
	~AAssetWriter();

protected:
	std::string m_assetSavePath;
	std::set<std::string> m_loadedAssetName;

public:
	bool IsAssetNotLoaded(const std::string& assetName) const;

public:
	virtual std::unordered_map<EAssetType, std::vector<AAsset*>> SaveAsAssets(const std::string& filePath) = 0;
	virtual bool IsAcceptableFilePath(const std::string& filePath) const = 0;

public:
	std::string GetRelativePathFromSavePath(const std::string& path) const;

protected:
	void SaveAssets(const EAssetType& assetType, const std::vector<AAsset*>& assets) const;
};

