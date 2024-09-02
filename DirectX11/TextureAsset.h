#pragma once
#include "AAssetFile.h"
#include <memory>

class TextureAsset : public AAssetFile
{
public:
	TextureAsset(const std::string& AssetNameIn);
	virtual ~TextureAsset();

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

