#pragma once

class ComponentEntity;
class AComponent;
class AMeshComponent;
class StaticMeshComponent;
class SkeletalMeshComponent;
class CameraComponent;
class SphereCollisionComponent;
class OrientedBoxCollisionComponent;

class IComponentVisitor
{
public:
	virtual ~IComponentVisitor() = default;

public:
	virtual void Visit(StaticMeshComponent* staticModelComponent) = 0;
	virtual void Visit(SkeletalMeshComponent* skeletalModelComponent) = 0;

public:
	virtual void Visit(CameraComponent* cameraComponent) = 0;

public:
	virtual void Visit(SphereCollisionComponent* sphereCollisionComponent) = 0;
	virtual void Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent) = 0;
};

