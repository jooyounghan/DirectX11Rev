#include "MaterialAsset.h"
#include "BaseTextureAsset.h"
#include "AssetManager.h"

using namespace std;

MaterialAsset::MaterialAsset(const std::string& AssetNameIn, const bool& LoadFromAsset)
	: AAssetFile(LoadFromAsset ? AssetNameIn : AssetNameIn + AAssetFile::AssetTypeToSuffix[(EAssetType::Material)], EAssetType::Material)
{
}

MaterialAsset::~MaterialAsset()
{
}

void MaterialAsset::Serialize()
{
	FILE* OutputAssetFile = nullptr;
	string OutputAssetFilePath = DefaultOpenFileHelper(MaterialAssetOutPath, OutputAssetFile);

	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);

		SerializeAssetNameHelper(OutputAssetFile, AmbientOcculusionTextureAsset);
		SerializeAssetNameHelper(OutputAssetFile, SpecularTextureAsset);
		SerializeAssetNameHelper(OutputAssetFile, DiffuseTextureAsset);
		SerializeAssetNameHelper(OutputAssetFile, RoughnessTextureAsset);
		SerializeAssetNameHelper(OutputAssetFile, MetalicTextureAsset);
		SerializeAssetNameHelper(OutputAssetFile, NormalTextureAsset);
		SerializeAssetNameHelper(OutputAssetFile, HeightTextureAsset);
		SerializeAssetNameHelper(OutputAssetFile, EmissiveTextureAsset);

		fclose(OutputAssetFile);
	}

	
}

void MaterialAsset::Deserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	string AmbientOcculusionTextureAssetName;
	string SpecularTextureAssetName;
	string DiffuseTextureAssetName;
	string RoughnessTextureAssetName;
	string MetalicTextureAssetName;
	string NormalTextureAssetName;
	string HeightTextureAssetName;
	string EmissiveTextureAssetName;

	AAssetFile::DeserializeString(AmbientOcculusionTextureAssetName, FileIn);
	AAssetFile::DeserializeString(SpecularTextureAssetName, FileIn);
	AAssetFile::DeserializeString(DiffuseTextureAssetName, FileIn);
	AAssetFile::DeserializeString(RoughnessTextureAssetName, FileIn);
	AAssetFile::DeserializeString(MetalicTextureAssetName, FileIn);
	AAssetFile::DeserializeString(NormalTextureAssetName, FileIn);
	AAssetFile::DeserializeString(HeightTextureAssetName, FileIn);
	AAssetFile::DeserializeString(EmissiveTextureAssetName, FileIn);

	AmbientOcculusionTextureAsset = AssetManagerIn->GetManagingBasicTexture(AmbientOcculusionTextureAssetName);
	SpecularTextureAsset = AssetManagerIn->GetManagingBasicTexture(SpecularTextureAssetName);
	DiffuseTextureAsset = AssetManagerIn->GetManagingBasicTexture(DiffuseTextureAssetName);
	RoughnessTextureAsset = AssetManagerIn->GetManagingBasicTexture(RoughnessTextureAssetName);
	MetalicTextureAsset = AssetManagerIn->GetManagingBasicTexture(MetalicTextureAssetName);
	NormalTextureAsset = AssetManagerIn->GetManagingBasicTexture(NormalTextureAssetName);
	HeightTextureAsset = AssetManagerIn->GetManagingBasicTexture(HeightTextureAssetName);
	EmissiveTextureAsset = AssetManagerIn->GetManagingBasicTexture(EmissiveTextureAssetName);
}

void MaterialAsset::SerializeAssetNameHelper(FILE* FileIn, std::shared_ptr<BaseTextureAsset> BasicTextureAssetIn)
{
	if (BasicTextureAssetIn != nullptr)
	{
		SerializeString(BasicTextureAssetIn->GetAssetName(), FileIn);
	}
	else
	{
		SerializeString(string(), FileIn);
	}
}
