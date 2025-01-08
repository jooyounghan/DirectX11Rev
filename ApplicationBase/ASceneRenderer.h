#pragma once
#include "IComponentVisitor.h"

struct ID3D11DeviceContext;
class ComponentPSOManager;

class ASceneRenderer : public IComponentVisitor
{
public:
	ASceneRenderer(
		ID3D11DeviceContext* const* deviceContextAddress, 
		ComponentPSOManager* componentPsoManager
	);
	~ASceneRenderer() override = default;

protected:
	ID3D11DeviceContext* const* m_deviceContextAddress = nullptr;
	ComponentPSOManager* m_componentPsoManagerCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshAsset) = 0;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshAsset) = 0;

public:
	virtual void Visit(CameraComponent* cameraComponent) = 0;
};

