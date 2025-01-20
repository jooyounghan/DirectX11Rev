#include "AssetUpdater.h"

#include "AssetManager.h"

#include "ConstantBuffer.h"
#include "DynamicBuffer.h"

AssetUpdater::AssetUpdater(AssetManager* assetManager, ID3D11DeviceContext* deviceContext)
	: m_assetManager(assetManager), m_deviceContextCached(deviceContext)
{
}

void AssetUpdater::Visit(StaticMeshAsset* staticMeshAsset)
{
}

void AssetUpdater::Visit(SkeletalMeshAsset* skeletalMeshAsset)
{
}

void AssetUpdater::Visit(BaseTextureAsset* baseTextureAsset)
{
}

void AssetUpdater::Visit(ScratchTextureAsset* scratchTextureAsset)
{
}

void AssetUpdater::Visit(ModelMaterialAsset* modelMaterialAsset)
{
	DynamicBuffer* modelTextureSettingBuffer = modelMaterialAsset->GetModelTextureSettingBuffer();
	modelTextureSettingBuffer->Upload(m_deviceContextCached);
}

void AssetUpdater::Visit(IBLMaterialAsset* iblMaterialAsset)
{
	DynamicBuffer* iBLToneMappingBuffer = iblMaterialAsset->GetIBLToneMappingBuffer();
	iBLToneMappingBuffer->Upload(m_deviceContextCached);
}

void AssetUpdater::Visit(BoneAsset* boneAsset)
{
}

void AssetUpdater::Visit(AnimationAsset* animationAsset)
{
}
