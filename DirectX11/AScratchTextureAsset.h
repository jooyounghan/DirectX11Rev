#pragma once
#include "ATextureAsset.h"
#include "DirectXTex.h"


class AScratchTextureAsset : public ATextureAsset
{
public:
	AScratchTextureAsset(
		const std::string& AssetNameIn,
		const std::string& AssetTypeIn
	);
	AScratchTextureAsset(
		const std::string& AssetNameIn,
		const std::string& AssetTypeIn,
		const DirectX::ScratchImage& scratch,
		const DirectX::TexMetadata& metaData
	);

private:
	virtual void CreateTexture(
		const std::vector<uint8_t*>& ImageBufferPerArray,
		const std::vector<size_t>& RowPitches
	) = 0;

public:
	virtual std::vector<std::vector<uint8_t>> CompressDataArray(
		const std::vector<uint8_t*>& DecompressedBufferPerArrayIn,
		const std::vector<size_t>& OriginalSizePerArray,
		std::vector<size_t>& CompressedSizePerArrayOut
	) override;

	virtual std::vector<std::vector<uint8_t>> DecompressDataArray(
		const std::vector<uint8_t*>& CompressedBufferPerArrayIn,
		const std::vector<size_t>& CompressedSizePerArray,
		const std::vector<size_t>& OriginalSizePerArray
	) override;
};

