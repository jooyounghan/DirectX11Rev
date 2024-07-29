#pragma once
#include "Serializable.h"
#include "HeaderHelper.h"
#include <array>

enum class EAssetType
{
	None,

	// Mesh
	StaticMesh,
	SkeletalMesh,

	// Bone
	Bone,

	// Texture
	Texture,

	// Animation
	Animation,

	NumAssetType,
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

	// Texture
	"_Texture",

	// Animation
	"_Animation"
};

constexpr const char* AssetOutPath = ".\\Assets\\";
constexpr const char* AssetMapOutPath = ".\\Assets\\Maps\\";
constexpr const char* AssetExtension = ".Asset";

struct ID3D11Device;

class IAssetFile : public ISerializable
{
public:
	IAssetFile(const std::string& AssetNameIn, EAssetType AssetTypeIn);
	virtual ~IAssetFile();

protected:
	std::string AssetName;
	EAssetType AssetType;
	MakeGetter(AssetName);
	MakeGetter(AssetType);

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") = 0;
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn) = 0;

public:
	FILE* DefaultOpenFile(const std::string& OutputAdditionalPath);
	void SerializeHeader(FILE* FileIn);
};

