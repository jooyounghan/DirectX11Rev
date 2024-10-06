#pragma once
#include "ATextureAsset.h"

class BasicTextureAsset : public ATextureAsset
{
public:
	BasicTextureAsset(const std::string& AssetNameIn);
	BasicTextureAsset(
		const std::string& AssetNameIn, 
		uint8_t* ImageBufferIn, 
		const UINT& WidthIn, 
		const UINT& HeightIn
	);
	virtual ~BasicTextureAsset();

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

