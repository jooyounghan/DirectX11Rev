#pragma once
#include "imgui.h"
#include "IComponentVisitor.h"
#include "ImGuiComboBox.h"

class AMeshComponent;
class AViewComponent;

class ComponentEntity;
class LightEntity;

class AssetManager;
class ComponentManager;

enum class EComponentEntityType
{
	ENTITY_POSITION,
	ENTITY_ANGLE,
	ENTITY_SCALE
};

class ComponentHandler : public IComponentVisitor
{
public:
	ComponentHandler(ComponentManager* componentManager);
	~ComponentHandler() override = default;

protected:
	AComponent* m_lastHandledComponent = nullptr;
	ComponentManager* m_componentManagerCached = nullptr;

private:
	ImGuiComboBox m_absRelativeComboPosition;
	ImGuiComboBox m_absRelativeComboAngle;
	ImGuiComboBox m_absRelativeComboScale;
	ImGuiComboBox m_animationPreview;
	std::string m_selectedAnimationName;

private:
	static bool IsPositionAbsolute;
	static bool IsAngleAbsolute;
	static bool IsScaleAbsolute;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

public:
	virtual void Visit(SphereCollisionComponent* sphereCollisionComponent) override;
	virtual void Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent) override;

public:
	virtual void Visit(SpotLightComponent* spotLightComponent) override;
	virtual void Visit(PointLightComponent* pointLightComponent) override;

private:
	void HandleComponentName(
		AComponent* comopnent, 
		const std::string& componentDescription
	);
	void HandleComponentTransformation(
		AComponent* component,
		const bool& isHandlePosition,
		const bool& isHandleAngle,
		const bool& isHandleScale
	);
	void HandleAnimationPlayer(SkeletalMeshComponent* skeletalMeshComponent);
	void HandleMeshComponent(AMeshComponent* meshComponent);
	void HandleSphereCollisionComponent(SphereCollisionComponent* sphereCollisionComponent);
	void HandleViewComponent(AViewComponent* viewComponent);
	void HandleOrientedCollisionComponent(OrientedBoxCollisionComponent* orientedBoxCollisionComponent);
	void HandleLightEntity(AComponent* component, LightEntity* lightEntity, const bool& isHandleSpotPower);

private:
	void RenderTransformationEntity(
		const char* groupID, const char* entityName, 
		AComponent* component, const EComponentEntityType& entityType,
		const float& valueSpeed, const float& minValue, const float& maxValue
	);
};

