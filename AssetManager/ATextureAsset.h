#pragma once
#include "Asset.h"
#include <vector>

class ATextureAsset : public AAsset
{
public:
	ATextureAsset() = default;
	ATextureAsset(
		const std::string& assetName,
		const unsigned int& widthIn,
		const unsigned int& heightIn,
		const unsigned int& arraysizeIn
	);
	virtual ~ATextureAsset();

protected:
	unsigned int m_width = 0;
	unsigned int m_height = 0;
	unsigned int m_arraySize = 0;

protected:
	std::vector<size_t> m_originalSizePerArray;
	std::vector<size_t> m_compressedSizePerArray;

protected:
	std::vector<std::vector<uint8_t>> m_compressedBufferPerArray;

public:
	std::vector<std::vector<uint8_t>> CompressDataArray(const std::vector<std::vector<uint8_t>>& originalBufferPerArray);
	std::vector<std::vector<uint8_t>> DecompressDataArray();

public:
	virtual std::vector<uint32_t> GetRowPitchArray() = 0;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

