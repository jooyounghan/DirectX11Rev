#include "EXRTextureAsset.h"

#include "DefineUtility.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

#include "DirectXTex.h"
#include "zlib.h"

using namespace std;
using namespace DirectX;

EXRTextureAsset::EXRTextureAsset(const std::string& AssetNameIn)
	: AAssetFile(AssetNameIn, EAssetType::EXRTexture) 
{
}

EXRTextureAsset::EXRTextureAsset(
	const std::string& AssetNameIn, 
	const ScratchImage& scratch, 
	const TexMetadata& metaData
)
	: AAssetFile(AssetNameIn + AssetSuffix[GetAssetTypeAsIndex(EAssetType::EXRTexture)], EAssetType::EXRTexture)
{	
	Width = metaData.width;
	Height = metaData.height;
	Format = metaData.format;
	OriginalSize = scratch.GetPixelsSize();

	const Image* pImage = scratch.GetImage(NULL, NULL, NULL);
	
	uint8_t* OriginalEXRFileData = (uint8_t*)malloc(OriginalSize);
	if (OriginalEXRFileData != nullptr)
	{
		memcpy(OriginalEXRFileData, scratch.GetPixels(), OriginalSize);
		CreateEXRTexture(OriginalEXRFileData, Width, Height, Format);

		CompressedBuffer = CompressData(OriginalEXRFileData, OriginalSize);		
		CompressedSize = CompressedBuffer.size();
	}

}

void EXRTextureAsset::CreateEXRTexture(uint8_t* ImageBufferIn, const UINT& WidthIn, const UINT& HeightIn, const DXGI_FORMAT& FormatIn)
{
	D3D11_TEXTURE2D_DESC Texture2DDesc;
	AutoZeroMemory(Texture2DDesc);
	Texture2DDesc.Width = WidthIn;
	Texture2DDesc.Height = HeightIn;
	Texture2DDesc.ArraySize = 1;
	Texture2DDesc.MipLevels = 0;
	Texture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Texture2DDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	Texture2DDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	Texture2DDesc.SampleDesc.Count = 1;
	Texture2DDesc.SampleDesc.Quality = 0;
	Texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	Texture2DDesc.Format = FormatIn;

	ID3D11Device* Device = App::GGraphicPipeline->GetDevice();
	ID3D11DeviceContext* DeviceContext = App::GGraphicPipeline->GetDeviceContext();

	AssertIfFailed(Device->CreateTexture2D(&Texture2DDesc, NULL, Texture2D.GetAddressOf()));

	DeviceContext->UpdateSubresource(Texture2D.Get(), 0, nullptr, ImageBufferIn, FormatIn == DXGI_FORMAT_R8G8B8A8_UNORM ? WidthIn * 4 : WidthIn * 8, NULL);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	AutoZeroMemory(SRVDesc);
	SRVDesc.Format = Texture2DDesc.Format;
	SRVDesc.ViewDimension = D3D10_1_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = -1;
	SRVDesc.Texture2D.MostDetailedMip = 0;

	Device->CreateShaderResourceView(Texture2D.Get(), &SRVDesc, SRV.GetAddressOf());
	DeviceContext->GenerateMips(SRV.Get());

	ThumbnailTexture2D = Texture2D;
	ThumbnailSRV = SRV;

	DeviceContext->PSSetShaderResources(0, 1, SRV.GetAddressOf());
}

vector<uint8_t> EXRTextureAsset::CompressData(uint8_t* ImageBufferIn, const size_t& DataSizeIn)
{
	uLongf CompressedSize = compressBound(DataSizeIn);
	std::vector<uint8_t> CompressedData(CompressedSize);

	int Result = compress2(CompressedData.data(), &CompressedSize, ImageBufferIn, DataSizeIn, Z_BEST_COMPRESSION);
	CompressedData.resize(CompressedSize);
	return CompressedData;
}

std::vector<uint8_t> EXRTextureAsset::DecompressData(uint8_t* CompressedBufferIn, const size_t& CompressedSize, const size_t& OriginalSize)
{
	uLongf DecompressedSize = OriginalSize;
	std::vector<uint8_t> DecompressedData(OriginalSize);

	int Result = uncompress(DecompressedData.data(), &DecompressedSize, CompressedBufferIn, CompressedSize);
	return DecompressedData;
}

void EXRTextureAsset::Serialize(const std::string& OutputAdditionalPath)
{
	FILE* OutputAssetFile = DefaultOpenFile(OutputAdditionalPath);

	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);

		fwrite(&Width, sizeof(UINT), 1, OutputAssetFile);
		fwrite(&Height, sizeof(UINT), 1, OutputAssetFile);
		fwrite(&Format, sizeof(DXGI_FORMAT), 1, OutputAssetFile);

		fwrite(&OriginalSize, sizeof(size_t), 1, OutputAssetFile);
		fwrite(&CompressedSize, sizeof(size_t), 1, OutputAssetFile);
		fwrite(CompressedBuffer.data(), CompressedSize, 1, OutputAssetFile);

		CompressedBuffer.clear();
		fclose(OutputAssetFile);
	}
}

void EXRTextureAsset::Deserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	fread(&Width, sizeof(UINT), 1, FileIn);
	fread(&Height, sizeof(UINT), 1, FileIn);
	fread(&Format, sizeof(DXGI_FORMAT), 1, FileIn);

	fread(&OriginalSize, sizeof(size_t), 1, FileIn);
	fread(&CompressedSize, sizeof(size_t), 1, FileIn);

	CompressedBuffer.clear();
	CompressedBuffer.resize(CompressedSize);

	fread(CompressedBuffer.data(), CompressedSize, 1, FileIn);

	std::vector<uint8_t> DecompressedBuffer = DecompressData(CompressedBuffer.data(), CompressedSize, OriginalSize);
	CompressedBuffer.clear();

	CreateEXRTexture(DecompressedBuffer.data(), Width, Height, Format);
	CompressedBuffer.clear();
}
