#include "ImageFileToAssetWriter.h"
#include "StringHelper.h"

#include "BaseTextureAsset.h"
#include "ScratchTextureAsset.h"

#include <DirectXTexEXR.h>

#include <filesystem>
#include <stb_image.h>
#include <stb_image_write.h>

using namespace std;
using namespace std::filesystem;
using namespace DirectX;

string ImageFileToAssetWriter::ExrExtension = ".exr";
string ImageFileToAssetWriter::DdsExtension = ".dds";
string ImageFileToAssetWriter::PngExtension = ".png";
string ImageFileToAssetWriter::JpegExtension = ".jpeg";
string ImageFileToAssetWriter::JpgExtension = ".jpg";

vector<string> ImageFileToAssetWriter::ImageFileExtensions = { ExrExtension, DdsExtension, PngExtension, JpegExtension, JpgExtension };

ImageFileToAssetWriter::ImageFileToAssetWriter(const string& assetSavePath)
	: AAssetWriter(assetSavePath)
{
}

unordered_map<EAssetType, vector<AAsset*>> ImageFileToAssetWriter::SaveAsAssets(const string& filePath)
{
	unordered_map<EAssetType, vector<AAsset*>> writtenAssets;

	const string& fileName = path(filePath).stem().string();
	const string& extension = path(filePath).extension().string();
	
	if (IsAssetNotLoaded(fileName))
	{
		bool isAdded = false;
		if (extension == PngExtension || extension == JpegExtension || extension == JpgExtension)
		{
			isAdded = true;
			AAsset* baseTextureAsset = LoadBaseTexureAsset(filePath, fileName);
			writtenAssets[EAssetType::ASSET_TYPE_BASE_TEXTURE].emplace_back(baseTextureAsset);
		}
		else if (extension == ExrExtension)
		{
			isAdded = true;
			AAsset* scratchTextureAsset = LoadScratchTexureByEXRAsset(filePath, fileName);
			writtenAssets[EAssetType::ASSET_TYPE_SCRATCH_TEXTURE].emplace_back(scratchTextureAsset);


		}
		else if (extension == DdsExtension)
		{
			isAdded = true;
			AAsset* scratchTextureAsset = LoadScratchTexureByDDSAsset(filePath, fileName);
			writtenAssets[EAssetType::ASSET_TYPE_SCRATCH_TEXTURE].emplace_back(scratchTextureAsset);
		}
		else
		{

		}

		if (isAdded)
		{
			m_loadedAssetName.insert(fileName);
		}
	}


	for (auto& writtenAsset : writtenAssets)
	{
		const EAssetType& assetType = writtenAsset.first;
		const vector<AAsset*>& assets = writtenAsset.second;

		SaveAssets(m_assetSavePath, assetType, assets);
	}
	return writtenAssets;
}

bool ImageFileToAssetWriter::IsAcceptableFilePath(const string& filePath) const
{
	const string& extension = path(filePath).extension().string();
	return (find(ImageFileExtensions.begin(), ImageFileExtensions.end(), extension) != ImageFileExtensions.end());
}

AAsset* ImageFileToAssetWriter::LoadBaseTexureAsset(const string& filePath, const string& fileName) const
{
	FILE* fileHandle;
	fopen_s(&fileHandle, filePath.c_str(), "rb");

	BaseTextureAsset* result = nullptr;

	if (fileHandle != nullptr)
	{
		uint8_t* imageBuffer = nullptr;
		int widthOut, heightOut, channelOut;
		imageBuffer = stbi_load_from_file(fileHandle, &widthOut, &heightOut, &channelOut, 4);

		if (imageBuffer != nullptr)
		{
			result = new BaseTextureAsset(fileName, widthOut, heightOut, imageBuffer);
		}
		fclose(fileHandle);

	}
	else
	{
	}

	return result;
}

AAsset* ImageFileToAssetWriter::LoadScratchTexureByEXRAsset(const string& filePath, const string& fileName) const
{
	ScratchImage scratch;
	TexMetadata metaData;
	HRESULT hResult = LoadFromEXRFile(StringHelper::ConvertACPToWString(filePath).c_str(), &metaData, scratch);

	ScratchTextureAsset* result = nullptr;

	if (!FAILED(hResult))
	{
		result = new ScratchTextureAsset(fileName, scratch, metaData);
	}

	return result;
}

AAsset* ImageFileToAssetWriter::LoadScratchTexureByDDSAsset(const string& filePath, const string& fileName) const
{
	ScratchImage scratch;
	TexMetadata metaData;
	HRESULT hResult = LoadFromDDSFile(StringHelper::ConvertACPToWString(filePath).c_str(), DDS_FLAGS_NONE, &metaData, scratch);

	ScratchTextureAsset* result = nullptr;

	if (!FAILED(hResult))
	{
		result = new ScratchTextureAsset(fileName, scratch, metaData);
	}
	return result;
}
