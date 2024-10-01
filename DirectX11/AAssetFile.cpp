#include "AAssetFile.h"
#include <filesystem>

using namespace std;
using namespace filesystem;

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
	fwrite(&AssetType, sizeof(AssetType), 1, FileIn);
}


