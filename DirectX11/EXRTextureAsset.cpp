#include "EXRTextureAsset.h"

#include "DefineUtility.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"


using namespace std;
using namespace DirectX;

string EXRTextureAsset::EXRTextureAssetKind = "EXRTexture";

EXRTextureAsset::EXRTextureAsset(const string& AssetNameIn)
	: AScratchTextureAsset(AssetNameIn, EXRTextureAsset::EXRTextureAssetKind)
{
}

EXRTextureAsset::EXRTextureAsset(
	const string& AssetNameIn, 
	const ScratchImage& scratch, 
	const TexMetadata& metaData
)
	: AScratchTextureAsset(
		AssetNameIn + "_" + EXRTextureAsset::EXRTextureAssetKind,
		EXRTextureAsset::EXRTextureAssetKind, scratch, metaData
	)
{	
	Width = static_cast<UINT>(metaData.width);
	Height = static_cast<UINT>(metaData.height);
	Format = metaData.format;
	ArraySize = metaData.arraySize;

	vector<uint8_t*> ImageBufferPerArray;
	vector<size_t> RowPitches;

	for (size_t ArrayIdx = 0; ArrayIdx < ArraySize; ++ArrayIdx)
	{
		const Image* img = scratch.GetImage(0, ArrayIdx, 0);
		ImageBufferPerArray.push_back(img->pixels);
		RowPitches.push_back(img->rowPitch);
		OriginalSizePerArray.push_back(img->rowPitch * img->height);
	}
	CompressedSizePerArray.resize(ArraySize);

	CreateTexture(ImageBufferPerArray, RowPitches);
	CompressedBufferPerArray = CompressDataArray(ImageBufferPerArray, OriginalSizePerArray, CompressedSizePerArray);
}

void EXRTextureAsset::CreateTexture(
	const std::vector<uint8_t*>& ImageBufferPerArray,
	const std::vector<size_t>& RowPitches
)
{
	ID3D11DeviceContext* DeviceContext = App::GGraphicPipeline->GetDeviceContext();

	D3D11_TEXTURE2D_DESC Texture2DDesc = CreateTexture2D(
		ImageBufferPerArray, RowPitches,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		NULL,
		D3D11_RESOURCE_MISC_GENERATE_MIPS,
		D3D11_USAGE_DEFAULT
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = CreateSRV(
		Texture2DDesc
	);

	DeviceContext->PSSetShaderResources(0, 1, SRV.GetAddressOf());
}
