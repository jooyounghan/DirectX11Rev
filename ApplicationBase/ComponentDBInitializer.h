#pragma once
#include "IComponentVisitor.h"

#include <mysqlx/xdevapi.h>

struct ID3D11Device;
class ICollisionOption;

class ComponentDBInitializer : public IComponentVisitor
{
public:
	ComponentDBInitializer(mysqlx::Schema* schema);
	~ComponentDBInitializer() override = default;

protected:
	mysqlx::Schema* m_schemaCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

public:
	virtual void Visit(SphereCollisionComponent* sphereCollisionComponent) override;
	virtual void Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent) override;

private:
	void LoadModelMaterials(AMeshComponent* meshComponent);
	ICollisionOption* CreateCollisionOption(const uint32_t& collitionOptionID);
};

