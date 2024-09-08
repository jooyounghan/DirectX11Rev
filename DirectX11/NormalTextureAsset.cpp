#include "NormalTextureAsset.h"

#include "DefineUtility.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

using namespace std;

NormalTextureAsset::NormalTextureAsset(const std::string& AssetNameIn)
	: AAssetFile(AssetNameIn, EAssetType::NormalTexture)
{
}

NormalTextureAsset::NormalTextureAsset(
	const std::string& AssetNameIn, 
	uint8_t* ImageBufferIn, 
	const UINT& WidthIn, 
	const UINT& HeightIn
)
	: AAssetFile(AssetNameIn + AssetSuffix[GetAssetTypeAsIndex(EAssetType::NormalTexture)], EAssetType::NormalTexture)
{
	CreateTexture(ImageBufferIn, WidthIn, HeightIn);

	CompressedBuffer = stbi_write_png_to_mem(ImageBufferIn, WidthIn * 4, WidthIn, HeightIn, 4, &ImageSize);
}

NormalTextureAsset::~NormalTextureAsset()
{
}

void NormalTextureAsset::CreateTexture(uint8_t* ImageBufferIn, const UINT& WidthIn, const UINT& HeightIn)
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
	Texture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	ID3D11Device* Device = App::GGraphicPipeline->GetDevice();
	ID3D11DeviceContext* DeviceContext = App::GGraphicPipeline->GetDeviceContext();

	AssertIfFailed(Device->CreateTexture2D(&Texture2DDesc, NULL, Texture2D.GetAddressOf()));

	DeviceContext->UpdateSubresource(Texture2D.Get(), 0, nullptr, ImageBufferIn, WidthIn * 4, NULL);

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

void NormalTextureAsset::Serialize(const std::string& OutputAdditionalPath)
{
	FILE* OutputAssetFile = DefaultOpenFile(OutputAdditionalPath);

	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);

		fwrite(&ImageSize, sizeof(int), 1, OutputAssetFile);

		fwrite(CompressedBuffer, ImageSize, 1, OutputAssetFile);
		stbi_image_free(CompressedBuffer);
		CompressedBuffer = nullptr;

		fclose(OutputAssetFile);
	}
}

void NormalTextureAsset::Deserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	fread(&ImageSize, sizeof(int), 1, FileIn);

	CompressedBuffer = new uint8_t[ImageSize];
	fread(CompressedBuffer, ImageSize, 1, FileIn);

	int Width, Height, Channel;

	uint8_t* ImageBuffer = stbi_load_from_memory(CompressedBuffer, ImageSize, &Width, &Height, &Channel, 4);
	CreateTexture(ImageBuffer, Width, Height);
	stbi_image_free(ImageBuffer);
	stbi_image_free(CompressedBuffer);
}
