#include "AAssetFile.h"
#include <filesystem>

using namespace std;
using namespace filesystem;

AAssetFile::AAssetFile(
	const string& AssetNameIn, const string& AssetTypeIn
)
	: AssetName(AssetNameIn), AssetType(AssetTypeIn)
{
}

AAssetFile::~AAssetFile()
{
}

string AAssetFile::DefaultOpenFileHelper(const char* AssetFilePath, FILE*& FileOut)
{
	path AssetPath = path(AssetFilePath);

	if (!exists(AssetPath) && create_directories(AssetPath)) {/* Do Nothing But Make Directory */ };

	const string OutputFullPath = AssetFilePath + AssetName + AssetExtension;
	errno_t result = fopen_s(&FileOut, OutputFullPath.c_str(), "wb");

	return OutputFullPath;
}

void AAssetFile::SerializeHeader(FILE* FileIn)
{
	// Asset Name
	size_t AssetNameSize = AssetName.size();
	fwrite(&AssetNameSize, sizeof(size_t), 1, FileIn);
	fwrite(AssetName.c_str(), sizeof(char), AssetNameSize, FileIn);

	// Asset Type
	SerializeString(AssetType, FileIn);
}

void AAssetFile::SerializeString(const std::string& String, FILE* FileIn)
{
	// Object Name
	size_t NameCount = String.size();
	fwrite(&NameCount, sizeof(size_t), 1, FileIn);
	fwrite(String.c_str(), sizeof(char), NameCount, FileIn);
}

void AAssetFile::DeserializeString(std::string& String, FILE* FileIn)
{
	// Object Name
	size_t NameCount;
	fread(&NameCount, sizeof(size_t), 1, FileIn);
	String.resize(NameCount);
	fread(String.data(), sizeof(char), NameCount, FileIn);
}

