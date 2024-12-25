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
	virtual void Visit(StaticModelComponent* staticMeshAsset) override;
	virtual void Visit(SkeletalModelComponent* skeletalMeshAsset) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;
};

