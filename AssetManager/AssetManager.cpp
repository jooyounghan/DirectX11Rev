#include "pch.h"
#include "AssetManager.h"
#include "AssetInitializer.h"

#include "ModelFileToAssetWriter.h"
#include "ImageFileToAssetWriter.h"

using namespace std;


AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
	for (auto& assetWriterNameToWriters : m_assetWritersWithPath)
	{
		for (AAssetWriter* assetWriter : assetWriterNameToWriters.second)
		{
			delete assetWriter;
		}
	}

	for (auto& assetNameToAssets : m_assetNameToAssets)
	{
		for (auto& assetNameToAsset : assetNameToAssets.second)
		{
			delete assetNameToAsset.second;
		}
	}
	;
}

void AssetManager::RegisterAssetReadPath(const std::string& readPath)
{
	if (m_assetReadersWithPath.find(readPath) == m_assetReadersWithPath.end())
	{
		m_assetReadersWithPath.emplace(readPath, AssetReader(readPath));
	}
}

void AssetManager::RegisterAssetWritePath(const std::string& writePath)
{
	if (m_assetWritersWithPath.find(writePath) == m_assetWritersWithPath.end())
	{
		m_assetWritersWithPath[writePath].emplace_back(new ModelFileToAssetWriter(writePath));
		m_assetWritersWithPath[writePath].emplace_back(new ImageFileToAssetWriter(writePath));
	}
	
}

void AssetManager::PreloadFromResources()
{
	const vector<BaseTextureAsset*> bitmapResourceAssets = m_resourceManager.LoadBitmapResources();
	for (BaseTextureAsset* const bitmapResourceAsset : bitmapResourceAssets)
	{
		m_assetNameToAssets[EAssetType::ASSET_TYPE_RESOURCE].emplace(bitmapResourceAsset->GetAssetName(), bitmapResourceAsset);
	}
}

void AssetManager::PreloadFromDirectories(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{

	for (auto assetReaderWithPath : m_assetReadersWithPath)
	{
		AssetReader& assetReader = assetReaderWithPath.second;

		assetReader.UpdatePreloadArgs();
		unordered_map<EAssetType, vector<pair<string, AAsset*>>> assetTypesToLoadedAssetsWithPath = assetReader.GetLoadedAssetWithPath();

		for (const auto& assetTypeToLoadedAssetsWithPath : assetTypesToLoadedAssetsWithPath)
		{
			const EAssetType& assetType = assetTypeToLoadedAssetsWithPath.first;
			const vector<pair<string, AAsset*>> loadedAssetsWithPath = assetTypeToLoadedAssetsWithPath.second;
			for (auto& loadedAssetWithPath : loadedAssetsWithPath)
			{
				string assetPath = loadedAssetWithPath.first;
				AAsset* const asset = loadedAssetWithPath.second;

				AddAssetHelper(device, deviceContext, assetType, assetPath, asset);
			}
		}
	}
}

void AssetManager::WrtieFileAsAsset(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string filePath)
{
	for (auto assetWritersWithPath : m_assetWritersWithPath)
	{
		const vector<AAssetWriter*>& assetWriters = assetWritersWithPath.second;
		for (AAssetWriter* const assetWriter : assetWriters)
		{
			if (assetWriter->IsAcceptableFilePath(filePath))
			{
				const string relativeFilePath = assetWriter->GetRelativePathFromSavePath(filePath);

				const unordered_map<EAssetType, vector<AAsset*>> savedTypesToAssets = assetWriter->SaveAsAssets(filePath);

				for (const auto& savedTypeToAssets : savedTypesToAssets)
				{
					const EAssetType& assetType = savedTypeToAssets.first;
					const vector<AAsset*>& assets = savedTypeToAssets.second;
					for (AAsset* const asset : assets)
					{
						AddAssetHelper(device, deviceContext, assetType, relativeFilePath, asset);
					}
				}

			}
		}
	}
}

void AssetManager::AddAssetHelper(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const EAssetType& assetType, std::string assetPath, AAsset* asset)
{
	AssetInitializer assetGPUInitializer(this, device, deviceContext);
	asset->Accept(&assetGPUInitializer);

	InvokeAssetLoadedHandler(assetType, assetPath, asset);
	m_assetNameToAssets[assetType][asset->GetAssetName()] = asset;
}

void AssetManager::RegisterAssetLoadedHandler(const std::string& handlerName, const std::function<void(const EAssetType&, std::string, AAsset*)>& handler)
{
	OnAssetLoaded[handlerName] = handler;
}

void AssetManager::DeregisterAssetLoadedHandler(const std::string& handlerName)
{
	OnAssetLoaded.erase(handlerName);
}

void AssetManager::InvokeAssetLoadedHandler(const EAssetType& assetType, std::string assetPath, AAsset* asset)
{
	for (auto& handlerWithName : OnAssetLoaded)
	{
		auto& handler = handlerWithName.second;
		handler(assetType, assetPath, asset);
	}
}

BaseTextureAsset* const AssetManager::GetBaseTextureAsset(const string& assetName)
{
	return GetAssetHelper<BaseTextureAsset>(EAssetType::ASSET_TYPE_BASE_TEXTURE, assetName);
}

ScratchTextureAsset* const  AssetManager::GetScratchTextureAsset(const string& assetName)
{
	return GetAssetHelper<ScratchTextureAsset>(EAssetType::ASSET_TYPE_SCRATCH_TEXTURE, assetName);
}

StaticMeshAsset* const AssetManager::GetStaticMeshAsset(const string& assetName)
{
	return GetAssetHelper<StaticMeshAsset>(EAssetType::ASSET_TYPE_STATIC, assetName);
}

SkeletalMeshAsset* const AssetManager::GetSkeletalMeshAsset(const string& assetName)
{
	return GetAssetHelper<SkeletalMeshAsset>(EAssetType::ASSET_TYPE_SKELETAL, assetName);
}

ModelMaterialAsset* const AssetManager::GetModelMaterialAsset(const string& assetName)
{
	return GetAssetHelper<ModelMaterialAsset>(EAssetType::ASSET_TYPE_MODEL_MATERIAL, assetName);
}

IBLMaterialAsset* const AssetManager::GetIBLMaterialAsset(const string& assetName)
{
	return GetAssetHelper<IBLMaterialAsset>(EAssetType::ASSET_TYPE_IBL_MATERIAL, assetName);
}

BoneAsset* const AssetManager::GetBoneAsset(const string& assetName)
{
	return GetAssetHelper<BoneAsset>(EAssetType::ASSET_TYPE_BONE, assetName);
}

AnimationAsset* const AssetManager::GetAnimationAsset(const string& assetName)
{
	return GetAssetHelper<AnimationAsset>(EAssetType::ASSET_TYPE_ANIMATION, assetName);
}

BaseTextureAsset* const AssetManager::GetResourceAsset(const string& assetName)
{
	return GetAssetHelper<BaseTextureAsset>(EAssetType::ASSET_TYPE_RESOURCE, assetName);
}


template<typename T>
inline T* const AssetManager::GetAssetHelper(const EAssetType& asssetType, const string& assetName)
{
	const unordered_map<string, AAsset*>& assetNameToAssets = m_assetNameToAssets[asssetType];
	if (assetNameToAssets.find(assetName) != assetNameToAssets.end())
	{
		return (T*)(assetNameToAssets.at(assetName));
		//return dynamic_cast<T*>(assetNameToAssets.at(assetName));
	}
	return nullptr;
}
