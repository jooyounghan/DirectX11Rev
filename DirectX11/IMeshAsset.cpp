#include "IMeshAsset.h"

IMeshAsset::IMeshAsset(const std::string& AssetNameIn, EAssetType AssetTypeIn)
	: IAssetFile(AssetNameIn, AssetTypeIn)
{
}

IMeshAsset::~IMeshAsset()
{
}
