#include "pch.h"
#include "ATextureAsset.h"

using namespace std;

ATextureAsset::ATextureAsset(
	const string& assetName,
	const unsigned int& widthIn,
	const unsigned int& heightIn,
	const unsigned int& arraysizeIn
) 
	: AAsset(assetName), m_width(widthIn), m_height(heightIn), m_arraySize(arraysizeIn)
{

}

ATextureAsset::~ATextureAsset() 
{

}

vector<pair<uint32_t, vector<uint8_t>>> ATextureAsset::GetCompressedImageBuffers(const vector<vector<uint8_t>>& imageBuffers)
{
    vector<pair<uint32_t, vector<uint8_t>>> compressedImageBuffersWithOriginSize;

    for (const vector<uint8_t>& imageBuffer : imageBuffers)
    {
        uLong originSize = static_cast<uLong>(imageBuffer.size());
        uLong compressedSize = compressBound(originSize);

        vector<uint8_t> compressedData(compressedSize);
        int compressResult = compress2(
            compressedData.data(), &compressedSize,
            imageBuffer.data(),
            originSize, Z_BEST_COMPRESSION
        );
        compressedData.resize(compressedSize);
        compressedImageBuffersWithOriginSize.emplace_back(make_pair(imageBuffer.size(), move(compressedData)));
    }
    return compressedImageBuffersWithOriginSize;
}

vector<vector<uint8_t>> ATextureAsset::DecompressedImageBuffers(const vector<pair<uint32_t, vector<uint8_t>>>& compressedImageBuffersWithOriginSize)
{
    vector<vector<uint8_t>> imageBuffers;
    for (const pair<uint32_t, vector<uint8_t>>& compressedImageBufferWithSize : compressedImageBuffersWithOriginSize)
    {
        uLongf originalSize = static_cast<uLong>(compressedImageBufferWithSize.first);
        uLongf compressSize = static_cast<uLong>(compressedImageBufferWithSize.second.size());

        vector<uint8_t> imageBuffer(originalSize);
        int compressResult = uncompress(
            imageBuffer.data(),
            &originalSize,
            compressedImageBufferWithSize.second.data(),
            compressSize
        );
        imageBuffers.push_back(move(imageBuffer));
    }
    return imageBuffers;
}

void ATextureAsset::Serialize(FILE* fileIn) const
{
    AAsset::Serialize(fileIn);
    SerializeHelper::SerializeElement(m_width, fileIn);
    SerializeHelper::SerializeElement(m_height, fileIn);
    SerializeHelper::SerializeElement(m_arraySize, fileIn);

    vector<pair<uint32_t, vector<uint8_t>>> compressedImageBuffersWithSize = GetCompressedImageBuffers(m_imageBuffers);
    SerializeHelper::SerializeContainerSize(compressedImageBuffersWithSize, fileIn);
    for (auto& compressedImageBufferWithSize : compressedImageBuffersWithSize)
    {
        SerializeHelper::SerializeElement(compressedImageBufferWithSize.first, fileIn);
        SerializeHelper::SerializeSequenceContainer(compressedImageBufferWithSize.second, fileIn);
    }
}



void ATextureAsset::Deserialize(FILE* fileIn)
{
    AAsset::Deserialize(fileIn);
    m_width = DeserializeHelper::DeserializeElement<uint32_t>(fileIn);
    m_height = DeserializeHelper::DeserializeElement<uint32_t>(fileIn);
    m_arraySize = DeserializeHelper::DeserializeElement<uint32_t>(fileIn);
    vector<pair<uint32_t, vector<uint8_t>>> compressedImageBuffersWithSize;
    size_t containerSize = DeserializeHelper::DeserializeContainerSize(fileIn);
    for (size_t idx = 0; idx < containerSize; ++idx)
    {
        uint32_t imageBufferSize = DeserializeHelper::DeserializeElement<uint32_t>(fileIn);

        compressedImageBuffersWithSize.emplace_back(make_pair(imageBufferSize,
            DeserializeHelper::DeserializeSequenceContainer<vector<uint8_t>>(fileIn)
        ));
    }
    m_imageBuffers = DecompressedImageBuffers(compressedImageBuffersWithSize);
}