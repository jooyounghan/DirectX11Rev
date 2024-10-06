#include "MaterialAsset.h"
#include "BasicTextureAsset.h"
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
		SerializeAssetNameHelper(OutputAssetFile, NormalTextureAsset);
		SerializeAssetNameHelper(OutputAssetFile, HeightTextureAsset);

		fclose(OutputAssetFile);
	}
}

void MaterialAsset::Deserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	string AmbientOcculusionTextureAssetName;
	string SpecularTextureAssetName;
	string DiffuseTextureAssetName;
	string NormalTextureAssetName;
	string HeightTextureAssetName;

	AAssetFile::DeserializeString(AmbientOcculusionTextureAssetName, FileIn);
	AAssetFile::DeserializeString(SpecularTextureAssetName, FileIn);
	AAssetFile::DeserializeString(DiffuseTextureAssetName, FileIn);
	AAssetFile::DeserializeString(NormalTextureAssetName, FileIn);
	AAssetFile::DeserializeString(HeightTextureAssetName, FileIn);

	AmbientOcculusionTextureAsset = AssetManagerIn->GetManagingBasicTexture(AmbientOcculusionTextureAssetName);
	SpecularTextureAsset = AssetManagerIn->GetManagingBasicTexture(SpecularTextureAssetName);
	DiffuseTextureAsset = AssetManagerIn->GetManagingBasicTexture(DiffuseTextureAssetName);
	NormalTextureAsset = AssetManagerIn->GetManagingBasicTexture(NormalTextureAssetName);
	HeightTextureAsset = AssetManagerIn->GetManagingBasicTexture(HeightTextureAssetName);
}

void MaterialAsset::SerializeAssetNameHelper(FILE* FileIn, std::shared_ptr<BasicTextureAsset> BasicTextureAssetIn)
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
