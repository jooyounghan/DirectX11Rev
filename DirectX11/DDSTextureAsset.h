#pragma once
#include "AScratchTextureAsset.h"
#include "DirectXTex.h"

class DDSTextureAsset : public AScratchTextureAsset
{
public:
	DDSTextureAsset(const std::string& AssetNameIn);
	DDSTextureAsset(
		const std::string& AssetNameIn,
		const DirectX::ScratchImage& scratch,
		const DirectX::TexMetadata& metaData
	);

private:
	virtual void CreateTexture(
		const std::vector<uint8_t*>& ImageBufferPerArray
	) override;
};

