#include "MaterialAsset.h"
#include "BaseTextureAsset.h"
#include "AssetManager.h"

#include "GlobalVariable.h"
#include "UploadableBufferManager.h"

using namespace std;
using namespace DirectX;

string MaterialAsset::MaterialAssetKind = "Material";

MaterialAsset::MaterialAsset(const std::string& AssetNameIn, const bool& LoadFromAsset)
	: AAssetFile(LoadFromAsset ? AssetNameIn : AssetNameIn + "_" + MaterialAsset::MaterialAssetKind, MaterialAsset::MaterialAssetKind)
{
	MaterialDataBuffer = App::GUploadableBufferManager->CreateUploadableBuffer<UploadBuffer<SMaterialData>>();

	AutoZeroMemory(MaterialData);
	MaterialDataBuffer->SetStagedData(MaterialData);
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

		fwrite(&MaterialData.F0, sizeof(XMFLOAT3), 1, OutputAssetFile);
		fwrite(&MaterialData.HeightScale, sizeof(float), 1, OutputAssetFile);

		fclose(OutputAssetFile);
	}
}

void MaterialAsset::Deserialize(FILE* FileIn)
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

	XMFLOAT3 F0In;
	float HeightScaleIn;

	fread(&F0In, sizeof(XMFLOAT3), 1, FileIn);
	fread(&HeightScaleIn, sizeof(float), 1, FileIn);

	SetAmbientOcculusionTextureAsset(App::GAssetManager->GetManagingBasicTexture(AmbientOcculusionTextureAssetName));
	SetSpecularTextureAsset(App::GAssetManager->GetManagingBasicTexture(SpecularTextureAssetName));
	SetDiffuseTextureAsset(App::GAssetManager->GetManagingBasicTexture(DiffuseTextureAssetName));
	SetRoughnessTextureAsset(App::GAssetManager->GetManagingBasicTexture(RoughnessTextureAssetName));
	SetMetalicTextureAsset(App::GAssetManager->GetManagingBasicTexture(MetalicTextureAssetName));
	SetNormalTextureAsset(App::GAssetManager->GetManagingBasicTexture(NormalTextureAssetName));
	SetHeightTextureAsset(App::GAssetManager->GetManagingBasicTexture(HeightTextureAssetName));
	SetEmissiveTextureAsset(App::GAssetManager->GetManagingBasicTexture(EmissiveTextureAssetName));
	SetF0(F0In);
	SetHeightScale(HeightScaleIn);
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

void MaterialAsset::SetAmbientOcculusionTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn)
{
	AmbientOcculusionTextureAsset = AssetIn;
	MaterialData.IsAmbientOcculusionSet = (AssetIn != nullptr);
	MaterialDataBuffer->SetStagedData(MaterialData);
}

void MaterialAsset::SetSpecularTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn)
{
	SpecularTextureAsset = AssetIn;
	MaterialData.IsSpecularSet = (AssetIn != nullptr);
	MaterialDataBuffer->SetStagedData(MaterialData);
}

void MaterialAsset::SetDiffuseTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn)
{
	DiffuseTextureAsset = AssetIn;
	MaterialData.IsDiffuseSet = (AssetIn != nullptr);
	MaterialDataBuffer->SetStagedData(MaterialData);
}

void MaterialAsset::SetRoughnessTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn)
{
	RoughnessTextureAsset = AssetIn;
	MaterialData.IsRoughnessSet = (AssetIn != nullptr);
	MaterialDataBuffer->SetStagedData(MaterialData);
}

void MaterialAsset::SetMetalicTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn)
{
	MetalicTextureAsset = AssetIn;
	MaterialData.IsMetalicSet = (AssetIn != nullptr);
	MaterialDataBuffer->SetStagedData(MaterialData);
}

void MaterialAsset::SetNormalTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn)
{
	NormalTextureAsset = AssetIn;
	MaterialData.IsNormalSet = (AssetIn != nullptr);
	MaterialDataBuffer->SetStagedData(MaterialData);
}

void MaterialAsset::SetHeightTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn)
{
	HeightTextureAsset = AssetIn;
	MaterialData.IsHeightSet = (AssetIn != nullptr);
	MaterialDataBuffer->SetStagedData(MaterialData);
}

void MaterialAsset::SetEmissiveTextureAsset(const std::shared_ptr<BaseTextureAsset>& AssetIn)
{
	EmissiveTextureAsset = AssetIn;
	MaterialData.IsEmissiveSet = (AssetIn != nullptr);
	MaterialDataBuffer->SetStagedData(MaterialData);
}

void MaterialAsset::SetF0(const XMFLOAT3& F0In)
{
	MaterialData.F0 = F0In;
	MaterialDataBuffer->SetStagedData(MaterialData);
}

void MaterialAsset::SetHeightScale(const float& HeightScaleIn)
{
	MaterialData.HeightScale = HeightScaleIn;
	MaterialDataBuffer->SetStagedData(MaterialData);
}
