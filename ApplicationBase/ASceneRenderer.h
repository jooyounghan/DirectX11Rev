#pragma once
#include "PSOManager.h"
#include "PSOObject.h"
#include "IComponentVisitor.h"

class ASceneRenderer : public IComponentVisitor
{
public:
	ASceneRenderer(
		ID3D11DeviceContext** deviceContextAddress, 
		PSOManager* psoManager
	);

protected:
	ID3D11DeviceContext** m_deviceContextAddress = nullptr;
	PSOManager* m_psoManagerCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshAsset) = 0;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshAsset) = 0;

public:
	virtual void Visit(CameraComponent* cameraComponent) = 0;
};

