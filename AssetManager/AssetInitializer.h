#pragma once
#include "IAssetVisitor.h"

class AssetManager;
struct ID3D11Device;
struct ID3D11DeviceContext;

class AssetInitializer : public IAssetVisitor
{
public:
	AssetInitializer(AssetManager* assetManager, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~AssetInitializer() override = default;

protected:
	AssetManager* m_assetManager = nullptr;
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
	virtual void Visit(BoneAsset* boneAsset) override;
	virtual void Visit(AnimationAsset* animationAsset) override;
};

