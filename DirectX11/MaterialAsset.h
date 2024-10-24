#pragma once
#include "AAssetFile.h"

class BaseTextureAsset;

constexpr const char* MaterialAssetOutPath = ".\\Assets\\Material\\";

class MaterialAsset : public AAssetFile
{
public:
	MaterialAsset(const std::string& AssetNameIn, const bool& LoadFromAsset);
	virtual ~MaterialAsset();

public:
	static std::string MaterialAssetKind;

protected:
	std::shared_ptr<BaseTextureAsset> AmbientOcculusionTextureAsset;
	std::shared_ptr<BaseTextureAsset> SpecularTextureAsset;
	std::shared_ptr<BaseTextureAsset> DiffuseTextureAsset;
	std::shared_ptr<BaseTextureAsset> RoughnessTextureAsset;
	std::shared_ptr<BaseTextureAsset> MetalicTextureAsset;
	std::shared_ptr<BaseTextureAsset> NormalTextureAsset;
	std::shared_ptr<BaseTextureAsset> HeightTextureAsset;
	std::shared_ptr<BaseTextureAsset> EmissiveTextureAsset;

	
	MakeSetterGetter(AmbientOcculusionTextureAsset);
	MakeSetterGetter(SpecularTextureAsset);
	MakeSetterGetter(DiffuseTextureAsset);
	MakeSetterGetter(RoughnessTextureAsset);
	MakeSetterGetter(MetalicTextureAsset);
	MakeSetterGetter(NormalTextureAsset);
	MakeSetterGetter(HeightTextureAsset);
	MakeSetterGetter(EmissiveTextureAsset);

public:
	virtual void Serialize() override;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;

private:
	void SerializeAssetNameHelper(FILE* FileIn, std::shared_ptr<BaseTextureAsset> BasicTextureAssetIn);
};

