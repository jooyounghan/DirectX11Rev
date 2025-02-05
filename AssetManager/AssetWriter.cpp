
#include "AssetWriter.h"

using namespace std;

AAssetWriter::AAssetWriter(const string& assetSavePath)
	: m_assetSavePath(assetSavePath)
{
}

bool AAssetWriter::IsAssetNotLoaded(const std::string& assetName) const
{
    return m_loadedAssetName.find(assetName) == m_loadedAssetName.end();
}

std::string AAssetWriter::GetRelativePathFromSavePath(const std::string& path) const
{
    size_t lastSlashPos = m_assetSavePath.find_last_of('/');
    std::string basePathMarker = (lastSlashPos != std::string::npos) ?
        m_assetSavePath.substr(lastSlashPos + 1) : m_assetSavePath;

    auto pos = path.find(basePathMarker);
    if (pos == std::string::npos) 
    {
        return path;
    }

    return m_assetSavePath + path.substr(pos + basePathMarker.length());
}

void AAssetWriter::SaveAssets(const std::string& assetSavePath, const EAssetType& assetType, const std::vector<AAsset*>& assets)
{
    AssetWrapper assetWrapper;
    assetWrapper.SetAssetType(assetType);
    for (const AAsset* const asset : assets)
    {
        FILE* fileIn = nullptr;
        const string assetPathStr = assetSavePath + "/" + asset->GetAssetName() + AAsset::AssetExtension;
        fopen_s(&fileIn, assetPathStr.c_str(), "wb");
        if (fileIn != nullptr)
        {
            assetWrapper.Serialize(fileIn);
            asset->Serialize(fileIn);
            fclose(fileIn);
        }
    }
}