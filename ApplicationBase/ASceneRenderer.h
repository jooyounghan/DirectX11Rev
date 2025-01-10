#pragma once
#include "IComponentVisitor.h"
#include "ISceneVisitor.h"

struct ID3D11DeviceContext;
class ComponentPSOManager;

class ASceneRenderer : public IComponentVisitor, public ISceneVisitor
{
public:
	ASceneRenderer(
		ID3D11DeviceContext* const* deviceContextAddress,
		ComponentPSOManager* componentPsoManager,
		CameraComponent* const* cameraComponentAddress
	);
	~ASceneRenderer() override = default;

protected:
	ID3D11DeviceContext* const* m_deviceContextAddress = nullptr;
	ComponentPSOManager* m_componentPsoManagerCached = nullptr;
	CameraComponent* const* m_selectedCameraComponentAddressCached = nullptr;

public:
	virtual void Visit(Scene* scene) override final;

public:
	virtual void Visit(StaticMeshComponent* staticMeshAsset) = 0;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshAsset) = 0;

public:
	virtual void Visit(CameraComponent* cameraComponent) = 0;
};

