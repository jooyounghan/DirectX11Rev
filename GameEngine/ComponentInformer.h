#pragma once
#include "imgui.h"
#include "IComponentVisitor.h"
#include "ImGuiComboBox.h"

class ComponentEntity;
class AMeshComponent;
class AssetManager;
class ComponentManager;

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
	~ComponentInformer() override = default;

protected:
	AssetManager* m_assetManagerCached = nullptr;
	ComponentManager* m_componentManagerCached = nullptr;

private:
	ImGuiComboBox m_absRelativeComboPosition;
	ImGuiComboBox m_absRelativeComboAngle;
	ImGuiComboBox m_absRelativeComboScale;

private:
	static bool isPositionAbsolute;
	static bool isAngleAbsolute;
	static bool isScaleAbsolute;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

public:
	virtual void Visit(SphereCollisionComponent* sphereCollisionComponent) override;
	virtual void Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent) override;

private:
	void RenderComponentTransformation(AComponent* component);
	void RenderMeshComponent(AMeshComponent* meshComponent);

private:
	void RenderTransformationEntity(
		const char* groupID, const char* entityName, 
		AComponent* component, const EComponentEntityType& entityType,
		const float& valueSpeed, const float& minValue, const float& maxValue
	);
};

