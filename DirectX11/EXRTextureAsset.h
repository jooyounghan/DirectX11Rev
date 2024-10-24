#pragma once
#include "AScratchTextureAsset.h"

class EXRTextureAsset : public AScratchTextureAsset
{
public:
	EXRTextureAsset(const std::string& AssetNameIn);
	EXRTextureAsset(
		const std::string& AssetNameIn,
		const DirectX::ScratchImage& scratch,
		const DirectX::TexMetadata& metaData
	);

public:
	static std::string EXRTextureAssetKind;

private:
	virtual void CreateTexture(
		const std::vector<uint8_t*>& ImageBufferPerArray,
		const std::vector<size_t>& RowPitches
	) override;
};

