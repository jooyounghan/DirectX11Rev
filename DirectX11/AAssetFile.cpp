#include "AAssetFile.h"
#include <stdio.h>
#include <Windows.h>

using namespace std;

AAssetFile::AAssetFile(
	const string& AssetNameIn, EAssetType AssetTypeIn
)
	: AssetName(AssetNameIn), AssetType(AssetTypeIn)
{
}

AAssetFile::~AAssetFile()
{
}

FILE* AAssetFile::DefaultOpenFile(const std::string& OutputAdditionalPath)
{
	const string OutputPath = OutputAdditionalPath.empty() ?
		AssetOutPath : AssetOutPath + OutputAdditionalPath;

	const string OutputFullPath = OutputPath + AssetName + AssetExtension;

	CreateDirectoryA(OutputPath.c_str(), NULL);

	FILE* OutputAssetFile = nullptr;
	errno_t result = fopen_s(&OutputAssetFile, OutputFullPath.c_str(), "wb");

	return OutputAssetFile;
}

void AAssetFile::SerializeHeader(FILE* FileIn)
{
	// Asset Name
	size_t AssetNameSize = AssetName.size();
	fwrite(&AssetNameSize, sizeof(size_t), 1, FileIn);
	fwrite(AssetName.c_str(), sizeof(char), AssetNameSize, FileIn);

	// Asset Type
	fwrite(&AssetType, sizeof(AssetType), 1, FileIn);
}

