#pragma once
#include "Asset.h"

#include <vector>
#include <d3d11.h>

template<typename ...IsTextureOption>
class Texture2DInstance;
class SRVOption;
class RTVOption;

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
	~ATextureAsset() override = default;

protected:
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	uint32_t m_arraySize = 0;

public:
	inline const uint32_t& GetWidth() const { return m_width; }
	inline const uint32_t& GetHeight() const { return m_height; }
	inline const uint32_t& GetArraySize() const { return m_arraySize; }

protected:
	std::vector<std::vector<uint8_t>> m_imageBuffers;
	std::vector<uint32_t> m_imageBuffersSize;
	Texture2DInstance<SRVOption, RTVOption>* m_resource = nullptr;

public:
	inline void SetResource(Texture2DInstance<SRVOption, RTVOption>* resource) { m_resource = resource; }
	inline const std::vector<std::vector<uint8_t>> GetImageBuffers() { return m_imageBuffers; }

public:
	ID3D11Texture2D* GetTexture2D() const;
	ID3D11ShaderResourceView* GetSRV() const;
	ID3D11RenderTargetView* GetRTV() const;

public:
	virtual std::vector<uint32_t> GetRowPitchArray() = 0;

private:
	static std::vector<std::pair<uint32_t, std::vector<uint8_t>>> GetCompressedImageBuffers(const std::vector<std::vector<uint8_t>>& imageBuffers);
	static std::vector<std::vector<uint8_t>> DecompressedImageBuffers(const std::vector<std::pair<uint32_t, std::vector<uint8_t>>>& compressedImageBuffersWithSize);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

