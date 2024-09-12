#include "DDSTextureAsset.h"

#include "DefineUtility.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

#include "DDSTextureLoader11.h"

using namespace std;
using namespace DirectX;

DDSTextureAsset::DDSTextureAsset(const string& AssetNameIn)
	: AScratchTextureAsset(AssetNameIn, EAssetType::DDSTexture)
{
}

DDSTextureAsset::DDSTextureAsset(
	const string& AssetNameIn, 
	const DirectX::ScratchImage& scratch, 
	const DirectX::TexMetadata& metaData
)
	: AScratchTextureAsset(
		AssetNameIn + AssetSuffix[GetAssetTypeAsIndex(EAssetType::DDSTexture)], 
		EAssetType::DDSTexture, scratch, metaData
	)
{
	Width = static_cast<UINT>(metaData.width);
	Height = static_cast<UINT>(metaData.height);
	Format = metaData.format;
	ArraySize = metaData.arraySize;

	vector<uint8_t*> ImageBufferPerArray;
	for (size_t ArrayIdx = 0; ArrayIdx < ArraySize; ++ArrayIdx)
	{
		const Image* img = scratch.GetImage(0, ArrayIdx, 0);
		ImageBufferPerArray.push_back(img->pixels);
		OriginalSizePerArray.push_back(img->rowPitch * img->height);
	}
	CompressedSizePerArray.resize(ArraySize);

	CreateTexture(ImageBufferPerArray);
	CompressedBufferPerArray = CompressDataArray(ImageBufferPerArray, OriginalSizePerArray, CompressedSizePerArray);
}

void DDSTextureAsset::CreateTexture(
	const std::vector<uint8_t*>& ImageBufferPerArray
)
{
	ID3D11DeviceContext* DeviceContext = App::GGraphicPipeline->GetDeviceContext();

	D3D11_TEXTURE2D_DESC Texture2DDesc = CreateTexture2D(
		ImageBufferPerArray, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		NULL, D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE,
		D3D11_USAGE_DEFAULT
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = CreateSRV(
		Texture2DDesc
	);

	DeviceContext->PSSetShaderResources(1, 1, SRV.GetAddressOf());
}
