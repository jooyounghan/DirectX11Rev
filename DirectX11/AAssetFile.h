#pragma once
#include "ISerializable.h"
#include "HeaderHelper.h"
#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>
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

	// Material
	Material,
	EXRTexture,
	NormalTexture,

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

	// Material
	"_Material",
	"_EXRTexture",
	"_NormalTexture",

	// Animation
	"_Animation"
};

constexpr const char* AssetOutPath = ".\\Assets\\";
constexpr const char* MapAssetOutPath = ".\\Assets\\Maps\\";
constexpr const char* AssetExtension = ".Asset";

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

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> ThumbnailTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ThumbnailSRV;
	MakeComPtrGetter(ThumbnailTexture2D);
	MakeComPtrGetter(ThumbnailSRV);

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") = 0;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;

public:
	virtual FILE* DefaultOpenFile(const std::string& OutputAdditionalPath);
	void SerializeHeader(FILE* FileIn);
};

