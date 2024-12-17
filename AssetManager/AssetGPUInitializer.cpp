#include "pch.h"
#include "AssetGPUInitializer.h"

#include "BaseTextureAsset.h"
#include "ScratchTextureAsset.h"
#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"
#include "ModelMaterialAsset.h"
#include "IBLMaterialAsset.h"
#include "BoneAsset.h"
#include "AnimationAsset.h"
#include "MapAsset.h"

AssetGPUInitializer::AssetGPUInitializer(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: m_deviceCached(device), m_deviceContextCached(deviceContext)
{
}

void AssetGPUInitializer::Visit(StaticMeshAsset* staticMeshAsset)
{
	staticMeshAsset->InitializeGPUAsset(m_deviceCached, m_deviceContextCached);
}

void AssetGPUInitializer::Visit(SkeletalMeshAsset* skeletalMeshAsset)
{
	skeletalMeshAsset->InitializeGPUAsset(m_deviceCached, m_deviceContextCached);
}

void AssetGPUInitializer::Visit(BaseTextureAsset* baseTextureAsset)
{
	baseTextureAsset->InitializeGPUAsset(m_deviceCached, m_deviceContextCached);
}

void AssetGPUInitializer::Visit(ScratchTextureAsset* scratchTextureAsset)
{
	scratchTextureAsset->InitializeGPUAsset(m_deviceCached, m_deviceContextCached);
}

void AssetGPUInitializer::Visit(ModelMaterialAsset* modelMaterialAsset)
{
}

void AssetGPUInitializer::Visit(IBLMaterialAsset* iblMaterialAsset)
{
}

void AssetGPUInitializer::Visit(MapAsset* mapAsset)
{
}

void AssetGPUInitializer::Visit(BoneAsset* boneAsset)
{
}

void AssetGPUInitializer::Visit(AnimationAsset* animationAsset)
{
}
