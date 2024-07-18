#include "TextureAsset.h"

TextureAsset::TextureAsset(const std::string& AssetNameIn)
	: IAssetFile(AssetNameIn, EAssetType::Texture)
{
}

TextureAsset::~TextureAsset()
{
}

void TextureAsset::Serialize(const std::string& OutputAdditionalPath)
{
}

void TextureAsset::Deserialize(const std::string& InputFullPath, GraphicsPipeline* GraphicsPipelineInstance)
{
}
