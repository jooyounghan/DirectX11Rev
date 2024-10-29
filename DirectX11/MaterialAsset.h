#pragma once
#include "AAssetFile.h"
#include "UploadBuffer.h"

class BaseTextureAsset;

struct SMaterialData
{
	int IsAmbientOcculusionSet;
	int IsSpecularSet;
	int IsDiffuseSet;
	int IsRoughnessSet;
	int IsMetalicSet;
	int IsNormalSet;
	int IsHeightSet;
	int IsEmissiveSet;
	DirectX::XMFLOAT3 F0;
	int Dummy;
};

constexpr const char* MaterialAssetOutPath = ".\\Assets\\Material\\";

class MaterialAsset : public AAssetFile
{
public:
	MaterialAsset(const std::string& AssetNameIn, const bool& LoadFromAsset);
	virtual ~MaterialAsset();

public:
	static std::string MaterialAssetKind;

protected:
	SMaterialData MaterialData;
	std::shared_ptr<UploadBuffer<SMaterialData>> MaterialDataBuffer;
	MakeGetter(MaterialDataBuffer);

public:
	inline const DirectX::XMFLOAT3& GetF0() { return MaterialData.F0; }

protected:
	std::shared_ptr<BaseTextureAsset> AmbientOcculusionTextureAsset;
	std::shared_ptr<BaseTextureAsset> SpecularTextureAsset;
	std::shared_ptr<BaseTextureAsset> DiffuseTextureAsset;
	std::shared_ptr<BaseTextureAsset> RoughnessTextureAsset;
	std::shared_ptr<BaseTextureAsset> MetalicTextureAsset;
	std::shared_ptr<BaseTextureAsset> NormalTextureAsset;
	std::shared_ptr<BaseTextureAsset> HeightTextureAsset;
	std::shared_ptr<BaseTextureAsset> EmissiveTextureAsset;
	MakeGetter(AmbientOcculusionTextureAsset);
	MakeGetter(SpecularTextureAsset);
	MakeGetter(DiffuseTextureAsset);
	MakeGetter(RoughnessTextureAsset);
	MakeGetter(MetalicTextureAsset);
	MakeGetter(NormalTextureAsset);
	MakeGetter(HeightTextureAsset);
	MakeGetter(EmissiveTextureAsset);

public:
	void SetAmbientOcculusionTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn);
	void SetSpecularTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn);
	void SetDiffuseTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn);
	void SetRoughnessTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn);
	void SetMetalicTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn);
	void SetNormalTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn);
	void SetHeightTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn);
	void SetEmissiveTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn);
	void SetF0(const DirectX::XMFLOAT3& F0In);

public:
	virtual void Serialize() override;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;

private:
	void SerializeAssetNameHelper(FILE* FileIn, std::shared_ptr<BaseTextureAsset> BasicTextureAssetIn);
};

