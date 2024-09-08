#pragma once
#include "AAssetFile.h"
#include "HeaderHelper.h"

class HDRTextureAsset : public AAssetFile
{
public:
	HDRTextureAsset(const std::string& AssetNameIn) : AAssetFile(AssetNameIn, EAssetType::HDRTexture) {};

};

