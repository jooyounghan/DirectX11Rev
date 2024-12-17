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
	unsigned int m_width = 0;
	unsigned int m_height = 0;
	unsigned int m_arraySize = 0;

protected:
	std::vector<std::vector<uint8_t>> m_imageBuffers;

public:
	inline const std::vector<std::vector<uint8_t>>& GetImageBuffers() { return m_imageBuffers; }

public:
	virtual std::vector<uint32_t> GetRowPitchArray() = 0;

public:
	virtual void Serialize(FILE* fileIn) const override final;
	virtual void Deserialize(FILE* fileIn) override final;
};

