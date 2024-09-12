#include "AAssetFile.h"
#include <stdio.h>

using namespace std;

unordered_map<EAssetType, string> AAssetFile::AssetTypeToSuffix
{
	{ EAssetType::None,				"" },

	// Mesh
	{ EAssetType::BaseMesh,			"_BaseMesh"},
	{ EAssetType::StaticMesh,		"_StaticMesh"},
	{ EAssetType::SkeletalMesh,		"_SkeletalMesh" },

	// Bone
	{ EAssetType::Bone,				"_Bone" },

	// Map
	{ EAssetType::Map,				"_Map" },

	// Material
	{ EAssetType::Material,			"_Material" },
	{ EAssetType::NormalTexture,	"_NormalTexture" },
	{ EAssetType::EXRTexture,		"_EXRTexture" },
	{ EAssetType::DDSTexture,		"_DDSTexture" },

	// Animation
	{ EAssetType::Animation,		"_Animation"}
};

unordered_map<string, EAssetType> AAssetFile::AssetSuffixToType
{
	{ "", EAssetType::None },

	// Mesh
	{ "_BaseMesh", EAssetType::BaseMesh },
	{ "_StaticMesh", EAssetType::StaticMesh },
	{ "_SkeletalMesh" , EAssetType::SkeletalMesh },

	// Bone
	{ "_Bone", EAssetType::Bone },

	// Map
	{ "_Map", EAssetType::Map },

	// Material
	{ "_Material", EAssetType::Material },
	{ "_NormalTexture", EAssetType::NormalTexture },
	{ "_EXRTexture", EAssetType::EXRTexture },
	{ "_DDSTexture", EAssetType::DDSTexture },

	// Animation
	{ "_Animation", EAssetType::Animation }
};

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


