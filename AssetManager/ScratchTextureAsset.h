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
	virtual ~ScratchTextureAsset();

protected:
	std::vector<UINT> m_rowPitchPerArray;

protected:
	Texture2DInstance<SRVOption>* m_textureWithSRV = nullptr;

public:
	virtual std::vector<uint32_t> GetRowPitchArray() override;

public:
	virtual const ID3D11Texture2D* const GetTexture2D() override;
	virtual const ID3D11ShaderResourceView* const GetSRV() override;

public:
	virtual void InitializeGPUAsset(
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext
	) override;

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
