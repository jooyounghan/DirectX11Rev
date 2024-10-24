#pragma once
#include "ISerializable.h"
#include "HeaderHelper.h"
#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <unordered_map>
#include <memory>


constexpr const char* AssetOutPath = ".\\Assets\\";
constexpr const char* AssetExtension = ".Asset";

class AAssetFile : public ISerializable
{
public:
	AAssetFile(const std::string& AssetNameIn, const std::string& AssetTypeIn);
	virtual ~AAssetFile();

protected:
	std::string AssetName;
	std::string AssetType;
	MakeGetter(AssetName);
	MakeGetter(AssetType);

protected:
	bool IsModified = false;
	MakeGetter(IsModified);

public:
	inline void SetModified() { IsModified = true; }

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

