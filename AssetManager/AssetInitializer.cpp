
#include "AssetInitializer.h"
#include "AssetManager.h"

#include <d3d11.h>

AssetInitializer::AssetInitializer(AssetManager* assetManager, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: m_assetManager(assetManager), m_deviceCached(device), m_deviceContextCached(deviceContext)
{
}

void AssetInitializer::Visit(StaticMeshAsset* staticMeshAsset)
{
	staticMeshAsset->InitializeGPUAsset(m_deviceCached, m_deviceContextCached);
}

void AssetInitializer::Visit(SkeletalMeshAsset* skeletalMeshAsset)
{
	skeletalMeshAsset->UpdateBoneAsset(*m_assetManager);
	skeletalMeshAsset->InitializeGPUAsset(m_deviceCached, m_deviceContextCached);
}

void AssetInitializer::Visit(BaseTextureAsset* baseTextureAsset)
{
	baseTextureAsset->InitializeGPUAsset(m_deviceCached, m_deviceContextCached);
}

void AssetInitializer::Visit(ScratchTextureAsset* scratchTextureAsset)
{
	scratchTextureAsset->InitializeGPUAsset(m_deviceCached, m_deviceContextCached);
}

void AssetInitializer::Visit(ModelMaterialAsset* modelMaterialAsset)
{
	for (size_t materialTextureType = 0; materialTextureType < ModelMaterialTextureCount; ++materialTextureType)
	{
		modelMaterialAsset->UpdateModelBaseTextureAsset(
			static_cast<EModelMaterialTexture>(materialTextureType),
			*m_assetManager
		);
	}
	modelMaterialAsset->InitializeGPUAsset(m_deviceCached, m_deviceContextCached);
}

void AssetInitializer::Visit(IBLMaterialAsset* iblMaterialAsset)
{
	for (size_t materialTextureType = 0; materialTextureType < IBLMaterialTextureCount; ++materialTextureType)
	{
		iblMaterialAsset->UpdateIBLBaseTextureAsset(
			static_cast<EIBLMaterialTexture>(materialTextureType),
			*m_assetManager
		);
	}
	iblMaterialAsset->InitializeGPUAsset(m_deviceCached, m_deviceContextCached);
}


void AssetInitializer::Visit(BoneAsset* boneAsset)
{
}

void AssetInitializer::Visit(AnimationAsset* animationAsset)
{
}
