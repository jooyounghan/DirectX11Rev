#pragma once
#include "IComponentVisitor.h"
#include <d3d11.h>

class ComponentEntityInitializer : public IComponentVisitor
{
public:
	ComponentEntityInitializer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	virtual ~ComponentEntityInitializer() override = default;

protected:
	ID3D11Device* m_deviceCached = nullptr;
	ID3D11DeviceContext* m_deviceContextCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticModelComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalModelComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

private:
	void InitTransformationBuffer(ComponentEntity* componentEntity);
};

