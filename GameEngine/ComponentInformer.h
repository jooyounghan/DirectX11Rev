#pragma once
#include "imgui.h"
#include "IComponentVisitor.h"
#include "ComponentManager.h"

class ComponentEntity;
class AMeshComponent;

enum class EComponentEntityType
{
	ENTITY_POSITION,
	ENTITY_ANGLE,
	ENTITY_SCALE
};

class ComponentInformer : public IComponentVisitor
{
public:
	ComponentInformer(AssetManager* assetManager, ComponentManager* componentManager);

protected:
	AssetManager* m_assetManagerCached = nullptr;
	ComponentManager* m_componentManagerCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

private:
	void RenderComponentTransformation(AComponent* component);
	void RenderMeshComponent(AMeshComponent* meshComponent);

private:
	void RenderAbsoluteRelativeSelector(bool& isAbsolute, AComponent* component);
	void RenderTransformationEntity(
		bool& isAbsolute, const char* groupID,
		const char* entityName, AComponent* component, const EComponentEntityType& entityType,
		const float& valueSpeed, const float& minValue, const float& maxValue
	);
};

