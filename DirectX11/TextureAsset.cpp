#include "TextureAsset.h"

#include "DefineUtility.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

TextureAsset::TextureAsset(const std::string& AssetNameIn)
	: AAssetFile(AssetNameIn, EAssetType::Texture)
{
}

TextureAsset::TextureAsset(
	const std::string& AssetNameIn, 
	uint8_t* ImageBufferIn, 
	const UINT& WidthIn, 
	const UINT& HeightIn,
	const bool& IsHDR
)
	: AAssetFile(AssetNameIn, EAssetType::Texture)
{
	D3D11_TEXTURE2D_DESC Texture2DDesc;
	AutoZeroMemory(Texture2DDesc);
	Texture2DDesc.Width = WidthIn;
	Texture2DDesc.Height = HeightIn;
	Texture2DDesc.ArraySize = 1;

	Texture2DDesc.MipLevels = 0;
	Texture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Texture2DDesc.CPUAccessFlags = NULL;
	Texture2DDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	Texture2DDesc.SampleDesc.Count = 1;
	Texture2DDesc.SampleDesc.Quality = 0;
	Texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	Texture2DDesc.Format = IsHDR ? DXGI_FORMAT_R16G16B16A16_FLOAT : DXGI_FORMAT_R8G8B8A8_UNORM;

	D3D11_SUBRESOURCE_DATA SubresourceData;
	AutoZeroMemory(SubresourceData);
	SubresourceData.pSysMem = ImageBufferIn;
	SubresourceData.SysMemPitch = IsHDR ? WidthIn * 8 : WidthIn * 4;
	SubresourceData.SysMemSlicePitch = IsHDR ? WidthIn * HeightIn * 8 : WidthIn * HeightIn * 4;

	ID3D11Device* Device = App::GGraphicPipeline->GetDevice();
	HRESULT hResult = Device->CreateTexture2D(&Texture2DDesc, &SubresourceData, Texture2D.GetAddressOf());
}

TextureAsset::~TextureAsset()
{
}

void TextureAsset::Serialize(const std::string& OutputAdditionalPath)
{
}

void TextureAsset::Deserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
}
