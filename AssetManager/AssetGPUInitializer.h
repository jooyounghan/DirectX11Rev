#pragma once
#include <d3d11.h>
#include "IAssetVisitor.h"

class AssetGPUInitializer : public IAssetVisitor
{
public:
	AssetGPUInitializer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

protected:
	ID3D11Device* m_deviceCached = nullptr;
	ID3D11DeviceContext* m_deviceContextCached = nullptr;

public:
	virtual void Visit(StaticMeshAsset* staticMeshAsset) override;
	virtual void Visit(SkeletalMeshAsset* skeletalMeshAsset) override;

public:
	virtual void Visit(BaseTextureAsset* baseTextureAsset) override;
	virtual void Visit(ScratchTextureAsset* scratchTextureAsset) override;

public:
	virtual void Visit(ModelMaterialAsset* modelMaterialAsset) override;
	virtual void Visit(IBLMaterialAsset* iblMaterialAsset) override;

public:
	virtual void Visit(MapAsset* mapAsset) override;
	virtual void Visit(BoneAsset* boneAsset) override;
	virtual void Visit(AnimationAsset* animationAsset) override;
};

