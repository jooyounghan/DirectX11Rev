#include "ATextureAsset.h"

#include "DefineUtility.h"
#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

using namespace std;
using namespace DirectX;

D3D11_TEXTURE2D_DESC ATextureAsset::CreateTexture2D(
	const std::vector<uint8_t*>& ImageBufferPerArray, 
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
	Texture2DDesc.ArraySize = ArraySize;
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
	for (size_t idx = 0; idx < ImageBufferPerArray.size(); ++idx)
	{
		const uint8_t* ImageBuffer = ImageBufferPerArray[idx];
		DeviceContext->UpdateSubresource(
			Texture2D.Get(), D3D11CalcSubresource(0, idx, Texture2DDesc.MipLevels), nullptr, 
			ImageBuffer, Format == DXGI_FORMAT_R8G8B8A8_UNORM ? Width * 4 : Width * 8, NULL
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
	SRVDesc.ViewDimension = TextureDescIn.ArraySize > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DARRAY : D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2DArray.ArraySize = TextureDescIn.ArraySize;
	SRVDesc.Texture2DArray.MipLevels = TextureDescIn.MipLevels;
	SRVDesc.Texture2DArray.MostDetailedMip = 0;

	Device->CreateShaderResourceView(Texture2D.Get(), &SRVDesc, SRV.GetAddressOf());
	DeviceContext->GenerateMips(SRV.Get());

	ThumbnailSRV = SRV;
	return SRVDesc;
}

void ATextureAsset::Serialize(const std::string& OutputAdditionalPath)
{
	FILE* OutputAssetFile = DefaultOpenFile(OutputAdditionalPath);

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
	for (vector<uint8_t>& DecompressedBuffer : DecompressedBufferPerArray)
	{
		vDecompressedBufferPerArray.push_back(DecompressedBuffer.data());
	}

	CreateTexture(vDecompressedBufferPerArray);

	DecompressedBufferPerArray.clear();
}
