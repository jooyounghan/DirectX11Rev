#include "BaseTextureAsset.h"

#include "DefineUtility.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

using namespace std;

BaseTextureAsset::BaseTextureAsset(const string& AssetNameIn)
	: ATextureAsset(AssetNameIn, EAssetType::BaseTexture)
{
}

BaseTextureAsset::BaseTextureAsset(
	const string& AssetNameIn, 
	uint8_t* ImageBufferIn, 
	const UINT& WidthIn, 
	const UINT& HeightIn
)
	: ATextureAsset(AssetNameIn + AAssetFile::AssetTypeToSuffix[(EAssetType::BaseTexture)], EAssetType::BaseTexture)
{
	Width = WidthIn;
	Height = HeightIn;
	Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ArraySize = 1;

	OriginalSizePerArray.push_back(Width * Height * 4);
	CompressedSizePerArray.resize(ArraySize);

	vector<uint8_t*> ImageBufferPerArray{ ImageBufferIn };
	vector<size_t> RowPitches{ Width * 4 };
	CreateTexture(ImageBufferPerArray, RowPitches);

	CompressedBufferPerArray = CompressDataArray(ImageBufferPerArray, OriginalSizePerArray, CompressedSizePerArray);
}

BaseTextureAsset::~BaseTextureAsset()
{
}

void BaseTextureAsset::CreateTexture(
	const std::vector<uint8_t*>& ImageBufferPerArray,
	const std::vector<size_t>& RowPitches
)
{
	D3D11_TEXTURE2D_DESC Texture2DDesc = CreateTexture2D(
		ImageBufferPerArray, RowPitches, 
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		NULL, D3D11_RESOURCE_MISC_GENERATE_MIPS, D3D11_USAGE_DEFAULT
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = CreateSRV(
		Texture2DDesc
	);
}


vector<vector<uint8_t>> BaseTextureAsset::CompressDataArray(
	const vector<uint8_t*>& DecompressedBufferPerArrayIn,
	const vector<size_t>& OriginalSizePerArray,
	vector<size_t>& CompressedSizePerArrayOut
)
{
	vector<vector<uint8_t>> vResult;
	for (size_t ArrayIdx = 0; ArrayIdx < ArraySize; ++ArrayIdx)
	{
		const uint8_t* DecompressedBufferIn = DecompressedBufferPerArrayIn[ArrayIdx];
		size_t& CompressedSize = CompressedSizePerArrayOut[ArrayIdx];
		
		uint8_t* CompressedBufferPtr = stbi_write_png_to_mem(DecompressedBufferIn, Width * 4, Width, Height, 4, (int*) &CompressedSize);
		vector<uint8_t> Result(CompressedBufferPtr, CompressedBufferPtr + CompressedSize);
		stbi_image_free(CompressedBufferPtr);
		vResult.push_back(std::move(Result));
	}
	return vResult;
}

vector<vector<uint8_t>> BaseTextureAsset::DecompressDataArray(
	const std::vector<uint8_t*>& CompressedBufferPerArrayIn,
	const std::vector<size_t>& CompressedSizePerArray,
	const std::vector<size_t>& OriginalSizePerArray
)
{
	vector<vector<uint8_t>> vResult;
	for (size_t ArrayIdx = 0; ArrayIdx < ArraySize; ++ArrayIdx)
	{
		int iWidth, iHeight, iChannel;

		const uint8_t* CompressedBufferIn = CompressedBufferPerArrayIn[ArrayIdx];
		const int CompressedSize = static_cast<int>(CompressedSizePerArray[ArrayIdx]);
		const size_t& OriginalSize = OriginalSizePerArray[ArrayIdx];

		uint8_t* DecompressedBufferPtr = stbi_load_from_memory(CompressedBufferIn, CompressedSize, &iWidth, &iHeight, &iChannel, 4);
		vector<uint8_t> Result(DecompressedBufferPtr, DecompressedBufferPtr + OriginalSize);
		stbi_image_free(DecompressedBufferPtr);
		vResult.push_back(std::move(Result));
	}
	return vResult;
}