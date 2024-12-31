#pragma once

class StaticMeshAsset;
class SkeletalMeshAsset;

class BaseTextureAsset;
class ScratchTextureAsset;

class ModelMaterialAsset;
class IBLMaterialAsset;

class BoneAsset;
class AnimationAsset;

class IAssetVisitor
{
public:
	virtual ~IAssetVisitor() = default;

public:
	virtual void Visit(StaticMeshAsset* staticMeshAsset) = 0;
	virtual void Visit(SkeletalMeshAsset* skeletalMeshAsset) = 0;

public:
	virtual void Visit(BaseTextureAsset* baseTextureAsset) = 0;
	virtual void Visit(ScratchTextureAsset* scratchTextureAsset) = 0;

public:
	virtual void Visit(ModelMaterialAsset* modelMaterialAsset) = 0;
	virtual void Visit(IBLMaterialAsset* iblMaterialAsset) = 0;

public:
	virtual void Visit(BoneAsset* boneAsset) = 0;
	virtual void Visit(AnimationAsset* animationAsset) = 0;
};

