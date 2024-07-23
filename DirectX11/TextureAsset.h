#pragma once
#include "IAssetFile.h"
#include <memory>

class TextureAsset : public IAssetFile
{
public:
	TextureAsset(const std::string& AssetNameIn);
	virtual ~TextureAsset();

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "");
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn);
};

