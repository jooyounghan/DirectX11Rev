#pragma once
#include "IComponentVisitor.h"

#include <mysqlx/xdevapi.h>

struct ID3D11DeviceContext;

class ComponentUpdater : public IComponentVisitor
{
public:
	ComponentUpdater(
		ID3D11DeviceContext* const* deviceContextAddress, 
		mysqlx::Schema* schema
	);
	~ComponentUpdater() override = default;

protected:
	ID3D11DeviceContext* const* m_deviceContextAddress = nullptr;
	mysqlx::Schema* m_schemaCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

private:
	void UpdateComponent(AComponent* component);
	void UpdateMeshComponent(AMeshComponent* meshComponent);
};

