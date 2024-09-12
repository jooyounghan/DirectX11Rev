#pragma once
#include "ATextureAsset.h"

class NormalTextureAsset : public ATextureAsset
{
public:
	NormalTextureAsset(const std::string& AssetNameIn);
	NormalTextureAsset(
		const std::string& AssetNameIn, 
		uint8_t* ImageBufferIn, 
		const UINT& WidthIn, 
		const UINT& HeightIn
	);
	virtual ~NormalTextureAsset();

private:
	virtual void CreateTexture(
		const std::vector<uint8_t*>& ImageBufferPerArray,
		const std::vector<size_t>& RowPitches
	) override;

public:
	virtual std::vector<std::vector<uint8_t>> CompressDataArray(
		const std::vector<uint8_t*>& DecompressedBufferPerArrayIn,
		const std::vector<size_t>& OriginalSizePerArray,
		std::vector<size_t>& CompressedSizeOut
	) override;

	virtual std::vector<std::vector<uint8_t>> DecompressDataArray(
		const std::vector<uint8_t*>& CompressedBufferPerArrayIn,
		const std::vector<size_t>& CompressedSize,
		const std::vector<size_t>& OriginalSize
	) override;
};

