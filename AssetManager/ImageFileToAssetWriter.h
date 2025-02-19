#pragma once
#include "AssetWriter.h"

class ImageFileToAssetWriter : public AAssetWriter
{
public:
	ImageFileToAssetWriter(const std::string& assetSavePath);
	~ImageFileToAssetWriter() override = default;

protected:
	static std::string ExrExtension;
	static std::string DdsExtension;
	static std::string PngExtension;
	static std::string JpegExtension;
	static std::string JpgExtension;

public:
	static std::vector<std::string> ImageFileExtensions;

public:
	virtual std::unordered_map<EAssetType, std::vector<AAsset*>> SaveAsAssets(const std::string& filePath) override;
	virtual bool IsAcceptableFilePath(const std::string& filePath) const override;

private:
	AAsset* LoadBaseTexureAsset(const std::string& filePath, const std::string& fileName) const;
	AAsset* LoadScratchTexureByEXRAsset(const std::string& filePath, const std::string& fileName) const;
	AAsset* LoadScratchTexureByDDSAsset(const std::string& filePath, const std::string& fileName) const;
};

