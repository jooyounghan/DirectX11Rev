#pragma once
#include "AAssetFile.h"
#include "HeaderHelper.h"
#include "DirectXTex.h"
#include <vector>

class EXRTextureAsset : public AAssetFile
{
public:
	EXRTextureAsset(const std::string& AssetNameIn);
	EXRTextureAsset(
		const std::string& AssetNameIn,
		const DirectX::ScratchImage& scratch,
		const DirectX::TexMetadata& metaData
	);

private:
	void CreateEXRTexture(
		uint8_t* ImageBufferIn,
		const UINT& WidthIn,
		const UINT& HeightIn,
		const DXGI_FORMAT& FormatIn
	);

public:
	std::vector<uint8_t> CompressData(uint8_t* DecompressedBufferIn, const size_t& DataSizeIn);
	std::vector<uint8_t> DecompressData(uint8_t* CompressedBufferIn, const size_t& CompressedSize, const size_t& OriginalSize);

protected:
	UINT Width = 0;
	UINT Height = 0;
	DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN;

protected:
	size_t OriginalSize = 0;
	size_t CompressedSize = 0;

protected:
	std::vector<uint8_t> CompressedBuffer;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				Texture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	SRV;
	MakeComPtrGetter(Texture2D);
	MakeComPtrGetter(SRV);

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

