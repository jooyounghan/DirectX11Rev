#pragma once
#include "ISerializable.h"
#include "HeaderHelper.h"
#include <memory>

enum class EAssetType
{
	None,

	// Mesh
	StaticMesh,
	SkeletalMesh,

	// Bone
	Bone,

	// Map
	Map,

	// Texture
	Texture,

	// Animation
	Animation,

	NumAssetType
};

constexpr size_t GetAssetTypeAsIndex(EAssetType AssetType) { return static_cast<size_t>(AssetType); }

constexpr const char* AssetSuffix[GetAssetTypeAsIndex(EAssetType::NumAssetType)] =
{
	"",

	// Mesh
	"_StaticMesh",
	"_SkeletalMesh",

	// Bone
	"_Bone",

	// Map
	"_Map",

	// Texture
	"_Texture",

	// Animation
	"_Animation"
};

constexpr const char* AssetOutPath = ".\\Assets\\";
constexpr const char* MapAssetOutPath = ".\\Assets\\Maps\\";
constexpr const char* AssetExtension = ".Asset";

struct ID3D11Device;

class AAssetFile : public ISerializable
{
public:
	AAssetFile(const std::string& AssetNameIn, EAssetType AssetTypeIn);
	virtual ~AAssetFile();

protected:
	std::string AssetName;
	EAssetType AssetType;
	MakeGetter(AssetName);
	MakeGetter(AssetType);

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") = 0;
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn, AssetManager* AssetManagerIn) = 0;

public:
	virtual FILE* DefaultOpenFile(const std::string& OutputAdditionalPath);
	void SerializeHeader(FILE* FileIn);
};

