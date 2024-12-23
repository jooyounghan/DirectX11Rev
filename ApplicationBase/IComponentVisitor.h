#pragma once

class StaticModelComponent;
class SkeletalModelComponent;

class CameraComponent;

class IComponentVisitor
{
public:
	virtual void Visit(StaticModelComponent* staticMeshAsset) = 0;
	virtual void Visit(SkeletalModelComponent* skeletalMeshAsset) = 0;

public:
	virtual void Visit(CameraComponent* baseTextureAsset) = 0;
};

