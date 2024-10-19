#pragma once
#include "ISerializable.h"
#include "HeaderHelper.h"
#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <unordered_map>
#include <memory>

enum class EAssetType
{
	None,

	// Mesh
	BaseMesh,
	StaticMesh,
	SkeletalMesh,

	// Bone
	Bone,

	// Map
	Map,

	// Material
	Material,
	BaseTexture,
	EXRTexture,
	DDSTexture,

	// Animation
	Animation,

	NumAssetType
};

constexpr const char* AssetOutPath = ".\\Assets\\";
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

public:
	static std::unordered_map<EAssetType, std::string> AssetTypeToSuffix;
	static std::unordered_map<std::string, EAssetType> AssetSuffixToType;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> ThumbnailTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ThumbnailSRV;
	MakeComPtrGetter(ThumbnailTexture2D);
	MakeComPtrGetter(ThumbnailSRV);

public:
	virtual void Serialize() = 0;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;

protected:
	std::string DefaultOpenFileHelper(const char* AssetFilePath, FILE*& FileOut);
	void SerializeHeader(FILE* FileIn);

public:
	static void SerializeString(const std::string& String, FILE* FileIn);
	static void DeserializeString(std::string& String, FILE* FileIn);
};

