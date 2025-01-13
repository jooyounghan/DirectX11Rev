#pragma once
#include "ATextureAsset.h"

namespace DirectX 
{
	class ScratchImage;
	struct TexMetadata;
}

class ScratchTextureAsset : public ATextureAsset
{
public:
	ScratchTextureAsset() = default;
	ScratchTextureAsset(
		const std::string& assetName,
		const DirectX::ScratchImage& scratch,
		const DirectX::TexMetadata& metaData
	);
	~ScratchTextureAsset() override = default;

protected:
	UINT m_mipLevels = NULL;
	UINT m_miscFlag = NULL;
	DXGI_FORMAT m_format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	std::vector<UINT> m_rowPitchPerArray;

public:
	inline UINT GetMipLevels() const { return m_mipLevels; }
	inline UINT GetMiscFlag() const { return m_miscFlag; }
	inline DXGI_FORMAT GetFormat() const { return m_format; }
	virtual std::vector<uint32_t> GetRowPitchArray() override;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual void Accept(IAssetVisitor* visitor) override;
};

class IScratchTextureProvider
{
public:
	virtual ScratchTextureAsset* const GetScratchTextureAsset(
		const std::string& assetName
	) = 0;
};
