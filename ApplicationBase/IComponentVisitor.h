#pragma once

class StaticModelComponent;
class SkeletalModelComponent;

class CameraComponent;

class IComponentVisitor
{
public:
	virtual void Visit(StaticModelComponent* staticModelComponent) = 0;
	virtual void Visit(SkeletalModelComponent* skeletalModelComponent) = 0;

public:
	virtual void Visit(CameraComponent* cameraComponent) = 0;
};

