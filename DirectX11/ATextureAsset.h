#pragma once
#include "AAssetFile.h"
#include <vector>

constexpr const char* TextureAssetOutPath = ".\\Assets\\Texture\\";

class ATextureAsset : public AAssetFile
{
public:
	ATextureAsset(const std::string& AssetNameIn, const std::string& AssetTypeIn) : AAssetFile(AssetNameIn, AssetTypeIn) {};
	~ATextureAsset() {};

public:
	virtual void CreateTexture(
		const std::vector<uint8_t*>& ImageBufferPerArray,
		const std::vector<size_t>& RowPitches
	) = 0;

protected:
	UINT Width = 0;
	UINT Height = 0;
	DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN;

protected:
	size_t ArraySize = 0;
	std::vector<size_t> OriginalSizePerArray;
	std::vector<size_t> CompressedSizePerArray;

protected:
	std::vector<std::vector<uint8_t>> CompressedBufferPerArray;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				Texture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	SRV;
	MakeComPtrGetter(Texture2D);
	MakeComPtrGetter(SRV);

protected:
	virtual D3D11_TEXTURE2D_DESC CreateTexture2D(
		const std::vector<uint8_t*>& ImageBufferPerArray,
		const std::vector<size_t>& RowPitches,
		const UINT/*D3D11_BIND_FLAG*/& BindFlagIn,
		const UINT/*D3D11_CPU_ACCESS_FLAG*/& CPUAccessFlagIn,
		const UINT/*D3D11_RESOURCE_MISC_FLAG*/& MiscFlagIn,
		const D3D11_USAGE& UsageIn
	) final;

	virtual D3D11_SHADER_RESOURCE_VIEW_DESC CreateSRV(const D3D11_TEXTURE2D_DESC& TextureDescIn) final;

public:
	virtual std::vector<std::vector<uint8_t>> CompressDataArray(
		const std::vector<uint8_t*>& DecompressedBufferPerArrayIn, 
		const std::vector<size_t>& OriginalSizePerArray, 
		std::vector<size_t>& CompressedSizePerArrayOut
	) = 0;
	virtual std::vector<std::vector<uint8_t>> DecompressDataArray(
		const std::vector<uint8_t*>& CompressedBufferPerArrayIn, 
		const std::vector<size_t>& CompressedSizePerArray,
		const std::vector<size_t>& OriginalSizePerArray
	) = 0;

public:
	virtual void Serialize() override;
	virtual void Deserialize(FILE* FileIn) override;

};