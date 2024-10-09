#pragma once
#include "AAssetFile.h"

class BasicTextureAsset;

constexpr const char* MaterialAssetOutPath = ".\\Assets\\Material\\";

class MaterialAsset : public AAssetFile
{
public:
	MaterialAsset(const std::string& AssetNameIn, const bool& LoadFromAsset);
	virtual ~MaterialAsset();

protected:
	std::shared_ptr<BasicTextureAsset> AmbientOcculusionTextureAsset;
	std::shared_ptr<BasicTextureAsset> SpecularTextureAsset;
	std::shared_ptr<BasicTextureAsset> DiffuseTextureAsset;
	std::shared_ptr<BasicTextureAsset> RoughnessTextureAsset;
	std::shared_ptr<BasicTextureAsset> MetalicTextureAsset;
	std::shared_ptr<BasicTextureAsset> NormalTextureAsset;
	std::shared_ptr<BasicTextureAsset> HeightTextureAsset;
	std::shared_ptr<BasicTextureAsset> EmissiveTextureAsset;

	
	MakeSmartPtrSetterGetter(AmbientOcculusionTextureAsset);
	MakeSmartPtrSetterGetter(SpecularTextureAsset);
	MakeSmartPtrSetterGetter(DiffuseTextureAsset);
	MakeSmartPtrSetterGetter(RoughnessTextureAsset);
	MakeSmartPtrSetterGetter(MetalicTextureAsset);
	MakeSmartPtrSetterGetter(NormalTextureAsset);
	MakeSmartPtrSetterGetter(HeightTextureAsset);
	MakeSmartPtrSetterGetter(EmissiveTextureAsset);

public:
	virtual void Serialize() override;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;

private:
	void SerializeAssetNameHelper(FILE* FileIn, std::shared_ptr<BasicTextureAsset> BasicTextureAssetIn);
};

