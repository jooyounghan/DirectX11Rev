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
	DXGI_FORMAT m_format;
	std::vector<UINT> m_rowPitchPerArray;

public:
	virtual std::vector<uint32_t> GetRowPitchArray() override;

public:
	virtual void InitializeGPUAsset(
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext
	) override;

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
