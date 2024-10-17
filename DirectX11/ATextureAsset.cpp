#include "ATextureAsset.h"

#include "DefineUtility.h"
#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

using namespace std;
using namespace DirectX;

D3D11_TEXTURE2D_DESC ATextureAsset::CreateTexture2D(
	const std::vector<uint8_t*>& ImageBufferPerArray, 
	const vector<size_t>& RowPitches,
	const UINT/*D3D11_BIND_FLAG*/& BindFlagIn,
	const UINT/*D3D11_CPU_ACCESS_FLAG*/& CPUAccessFlagIn, 
	const UINT/*D3D11_RESOURCE_MISC_FLAG*/& MiscFlagIn,
	const D3D11_USAGE& UsageIn
)
{
	ID3D11Device* Device = App::GGraphicPipeline->GetDevice();
	ID3D11DeviceContext* DeviceContext = App::GGraphicPipeline->GetDeviceContext();

	D3D11_TEXTURE2D_DESC Texture2DDesc;
	AutoZeroMemory(Texture2DDesc);
	Texture2DDesc.Width = Width;
	Texture2DDesc.Height = Height;
	Texture2DDesc.ArraySize = static_cast<UINT>(ArraySize);
	Texture2DDesc.MipLevels = 0;
	Texture2DDesc.BindFlags = BindFlagIn;
	Texture2DDesc.CPUAccessFlags = CPUAccessFlagIn;
	Texture2DDesc.MiscFlags = MiscFlagIn;
	Texture2DDesc.SampleDesc.Count = 1;
	Texture2DDesc.SampleDesc.Quality = 0;
	Texture2DDesc.Usage = UsageIn;
	Texture2DDesc.Format = Format;
	AssertIfFailed(Device->CreateTexture2D(&Texture2DDesc, NULL, Texture2D.GetAddressOf()));

	Texture2D->GetDesc(&Texture2DDesc);
	for (size_t ArrayIdx = 0; ArrayIdx < ImageBufferPerArray.size(); ++ArrayIdx)
	{
		const uint8_t* ImageBuffer = ImageBufferPerArray[ArrayIdx];
		DeviceContext->UpdateSubresource(
			Texture2D.Get(), D3D11CalcSubresource(0, static_cast<UINT>(ArrayIdx), Texture2DDesc.MipLevels), nullptr,
			ImageBuffer, RowPitches[ArrayIdx], NULL
		);
	}

	ThumbnailTexture2D = Texture2D;
	return Texture2DDesc;
}

D3D11_SHADER_RESOURCE_VIEW_DESC ATextureAsset::CreateSRV(const D3D11_TEXTURE2D_DESC& TextureDescIn)
{
	ID3D11Device* Device = App::GGraphicPipeline->GetDevice();
	ID3D11DeviceContext* DeviceContext = App::GGraphicPipeline->GetDeviceContext();

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	AutoZeroMemory(SRVDesc);
	SRVDesc.Format = TextureDescIn.Format;

	if (TextureDescIn.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE)
	{
		if (TextureDescIn.ArraySize / 6 > 1)
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
			SRVDesc.TextureCubeArray.NumCubes = static_cast<UINT>(ArraySize / 6);
			SRVDesc.TextureCubeArray.MipLevels = TextureDescIn.MipLevels;
			SRVDesc.TextureCubeArray.MostDetailedMip = 0;
		}
		else
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			SRVDesc.TextureCube.MipLevels = TextureDescIn.MipLevels;
			SRVDesc.TextureCube.MostDetailedMip = 0;
		}
	}
	else
	{
		if (TextureDescIn.ArraySize > 1)
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			SRVDesc.Texture2DArray.ArraySize = ArraySize;
			SRVDesc.Texture2DArray.MipLevels = TextureDescIn.MipLevels;
			SRVDesc.Texture2DArray.MostDetailedMip = 0;
		}
		else
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MipLevels = TextureDescIn.MipLevels;
			SRVDesc.Texture2D.MostDetailedMip = 0;
		}
	}

	Device->CreateShaderResourceView(Texture2D.Get(), &SRVDesc, SRV.GetAddressOf());
	DeviceContext->GenerateMips(SRV.Get());

	ThumbnailSRV = SRV;
	return SRVDesc;
}

void ATextureAsset::Serialize()
{
	FILE* OutputAssetFile;
	string OutputAssetFilePath = DefaultOpenFileHelper(TextureAssetOutPath, OutputAssetFile);

	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);

		fwrite(&Width, sizeof(UINT), 1, OutputAssetFile);
		fwrite(&Height, sizeof(UINT), 1, OutputAssetFile);
		fwrite(&Format, sizeof(DXGI_FORMAT), 1, OutputAssetFile);

		fwrite(&ArraySize, sizeof(size_t), 1, OutputAssetFile);
		for (UINT ArrayIdx = 0; ArrayIdx < ArraySize; ++ArrayIdx)
		{
			fwrite(&OriginalSizePerArray[ArrayIdx], sizeof(size_t), 1, OutputAssetFile);
			fwrite(&CompressedSizePerArray[ArrayIdx], sizeof(size_t), 1, OutputAssetFile);
			fwrite(CompressedBufferPerArray[ArrayIdx].data(), CompressedSizePerArray[ArrayIdx], 1, OutputAssetFile);
		}

		CompressedBufferPerArray.clear();

		fclose(OutputAssetFile);
	}
}

void ATextureAsset::Deserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	fread(&Width, sizeof(UINT), 1, FileIn);
	fread(&Height, sizeof(UINT), 1, FileIn);
	fread(&Format, sizeof(DXGI_FORMAT), 1, FileIn);

	fread(&ArraySize, sizeof(size_t), 1, FileIn);
	OriginalSizePerArray.resize(ArraySize);
	CompressedSizePerArray.resize(ArraySize);
	CompressedBufferPerArray.resize(ArraySize);

	vector<uint8_t*> vCompressedBufferPerArray;

	for (UINT ArrayIdx = 0; ArrayIdx < ArraySize; ++ArrayIdx)
	{
		fread(&OriginalSizePerArray[ArrayIdx], sizeof(size_t), 1, FileIn);
		fread(&CompressedSizePerArray[ArrayIdx], sizeof(size_t), 1, FileIn);

		CompressedBufferPerArray[ArrayIdx].resize(CompressedSizePerArray[ArrayIdx]);

		fread(CompressedBufferPerArray[ArrayIdx].data(), CompressedSizePerArray[ArrayIdx], 1, FileIn);
		vCompressedBufferPerArray.push_back(CompressedBufferPerArray[ArrayIdx].data());
	}

	vector<vector<uint8_t>> DecompressedBufferPerArray = DecompressDataArray(vCompressedBufferPerArray, CompressedSizePerArray, OriginalSizePerArray);
	vector<uint8_t*> vDecompressedBufferPerArray;
	vector<size_t> RowPitches;
	for (vector<uint8_t>& DecompressedBuffer : DecompressedBufferPerArray)
	{
		vDecompressedBufferPerArray.push_back(DecompressedBuffer.data());
		RowPitches.push_back(DecompressedBuffer.size() / Height);
	}

	CreateTexture(vDecompressedBufferPerArray, RowPitches);

	DecompressedBufferPerArray.clear();
}
