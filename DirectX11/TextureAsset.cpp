#include "TextureAsset.h"

TextureAsset::TextureAsset(const std::string& AssetNameIn)
	: AAssetFile(AssetNameIn, EAssetType::Texture)
{
}

TextureAsset::~TextureAsset()
{
}

void TextureAsset::Serialize(const std::string& OutputAdditionalPath)
{
}

void TextureAsset::Deserialize(FILE* FileIn, ID3D11Device* DeviceIn, AssetManager* AssetManagerIn)
{
}
