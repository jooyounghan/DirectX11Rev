#pragma once
#include "PSOManager.h"
#include "PSOObject.h"
#include "IComponentVisitor.h"

class SceneRenderer : public IComponentVisitor
{
public:
	SceneRenderer(PSOManager* psoManager);

protected:
	PSOManager* m_psoManagerCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshAsset) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshAsset) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;
};

