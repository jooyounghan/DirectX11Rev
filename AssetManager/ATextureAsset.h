#pragma once
#include "Asset.h"
#include "ATextureGPUAsset.h"
#include "Texture2DInstance.h"
#include "SRVOption.h"

class ATextureAsset : public AAsset, public ATextureGPUAsset
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
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	uint32_t m_arraySize = 0;

protected:
	std::vector<std::vector<uint8_t>> m_imageBuffers;

protected:
	std::vector<uint32_t> m_imageBuffersSize;

public:
	inline const std::vector<std::vector<uint8_t>>& GetImageBuffers() { return m_imageBuffers; }

public:
	virtual std::vector<uint32_t> GetRowPitchArray() = 0;

private:
	static std::vector<std::pair<uint32_t, std::vector<uint8_t>>> GetCompressedImageBuffersWithOriginSize(const std::vector<std::vector<uint8_t>>& imageBuffers);
	static std::vector<std::vector<uint8_t>> DecompressedImageBuffers(const std::vector<std::pair<uint32_t, std::vector<uint8_t>>>& compressedImageBuffersWithSize);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

