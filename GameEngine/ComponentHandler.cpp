#include "ComponentHandler.h"

#include "AssetManager.h"
#include "ComponentManager.h"
#include "AnimationPlayer.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"
#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"
#include "SpotLightComponent.h"
#include "PointLightComponent.h"

#include "ImGuiComboBox.h"

#include <vector>
#include <string>

using namespace std;
using namespace ImGui;
using namespace DirectX;

bool ComponentHandler::IsPositionAbsolute = true;
bool ComponentHandler::IsAngleAbsolute = true;

ComponentHandler::ComponentHandler(ComponentManager* componentManager)
	: m_componentManagerCached(componentManager),
	m_absRelativeComboPosition("AbsRelativePositionCombo", "", ImGuiComboFlags_WidthFitPreview),
	m_absRelativeComboAngle("AbsRelativeAngleCombo", "", ImGuiComboFlags_WidthFitPreview),
	m_animationPreview("AnimationPreviewer", "Select Animtion")
{
	const vector<string> absRelativeStrings = { "Absolute", "Relative" };

	m_absRelativeComboPosition.SetSelectableItems(IsPositionAbsolute ? absRelativeStrings[0] : absRelativeStrings[1], absRelativeStrings);
	m_absRelativeComboAngle.SetSelectableItems(IsPositionAbsolute ? absRelativeStrings[0] : absRelativeStrings[1], absRelativeStrings);

	m_absRelativeComboPosition.OnSelChanged = [&](const size_t& idx, const string&) { IsPositionAbsolute = !static_cast<bool>(idx); };
	m_absRelativeComboAngle.OnSelChanged = [&](const size_t& idx, const string&) { IsAngleAbsolute = !static_cast<bool>(idx); };
	m_animationPreview.OnSelChanged = [&](const size_t&, const string& animationAssetName) { m_selectedAnimationName = animationAssetName; };
}

void ComponentHandler::Visit(StaticMeshComponent* staticMeshComponent)
{
	HandleComponentName(staticMeshComponent, "Static Model Component");
	HandleComponentTransformation(staticMeshComponent, true, true, true);
	HandleMeshComponent(staticMeshComponent);
}

void ComponentHandler::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
	HandleComponentName(skeletalMeshComponent, "Skeletal Model Component");
	HandleComponentTransformation(skeletalMeshComponent, true, true, true);
	HandleMeshComponent(skeletalMeshComponent);
	HandleAnimationPlayer(skeletalMeshComponent);
}

void ComponentHandler::Visit(CameraComponent* cameraComponent)
{
	bool isViewChanged = false;
	HandleComponentName(cameraComponent, "Camera Model Component");


	if (HandleComponentTransformation(cameraComponent, true, true, false))
	{
		isViewChanged = true;
	}

	SeparatorText("View Properties");
	Text("Fov Angle");
	PushID("FovAngle");
	float& fovAngle = const_cast<float&>(cameraComponent->GetFovAngle());
	if (DragFloat("", &fovAngle, 0.1f, 60.f, 160.f))
	{
		isViewChanged = true;
	}

	if (isViewChanged)
	{
		cameraComponent->UpdateViewEntity();
	}
	PopID();
}

void ComponentHandler::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
	HandleComponentName(sphereCollisionComponent, "Sphere Collision Component");
	HandleComponentTransformation(sphereCollisionComponent, true, false, false);
	HandleSphereCollisionComponent(sphereCollisionComponent);
}

void ComponentHandler::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	HandleComponentName(orientedBoxCollisionComponent, "Oriented-Box Collision Component");
	HandleComponentTransformation(orientedBoxCollisionComponent, true, true, false);
	HandleOrientedCollisionComponent(orientedBoxCollisionComponent);
}

void ComponentHandler::Visit(SpotLightComponent* spotLightComponent)
{
	bool isViewChanged = false;
	HandleComponentName(spotLightComponent, "Spot Light Component");
	if (HandleComponentTransformation(spotLightComponent, true, true, false))
	{
		isViewChanged = true;
	}

	SeparatorText("View Properties");
	Text("Fov Angle");
	PushID("FovAngle");
	float& fovAngle = const_cast<float&>(spotLightComponent->GetFovAngle());
	if (DragFloat("", &fovAngle, 0.1f, 60.f, 160.f))
	{		
		isViewChanged = true;
	}

	if (isViewChanged)
	{
		spotLightComponent->UpdateViewEntity();
	}
	PopID();

	HandleLightEntity(spotLightComponent, true);
}

void ComponentHandler::Visit(PointLightComponent* pointLightComponent)
{
	HandleComponentName(pointLightComponent, "Point Light Component");
	if (HandleComponentTransformation(pointLightComponent, true, true, false))
	{
		pointLightComponent->UpdatePointLightFrustums();
	}
	HandleLightEntity(pointLightComponent, false);
}

void ComponentHandler::HandleComponentName(AComponent* component, const string& componentDescription)
{
	if (m_lastHandledComponent != component)
	{
		m_lastHandledComponent = component;
		m_animationPreview.ResetSelection();
	}
	SeparatorText(component->GetComponentName().c_str());
	Text(componentDescription.c_str());
}

bool ComponentHandler::HandleComponentTransformation(
	AComponent* component,
	const bool& isHandlePosition,
	const bool& isHandleAngle ,
	const bool& isHandleScale
)
{
	bool result = false;
	SeparatorText("Transformation");

	static bool isAbsolutePosition = false;
	static bool isAbsoluteAngle = 0;
	static bool isAbsoluteScale = 0;

	AComponent* parentComponent = component->GetParentComponent();

	if (isHandlePosition)
	{
		result |= RenderTransformationEntity(
			"RenderPositionEntity", "Position", 
			component, EComponentEntityType::ENTITY_POSITION,
			1.f, -1E9f, 1E9f
		);
	}
	if (isHandleAngle)
	{
		result |= RenderTransformationEntity(
			"RenderAngleEntity", "Angle",
			component, EComponentEntityType::ENTITY_ANGLE,
			0.1f, -360.f, 360.f
		);
	}
	if (isHandleScale)
	{
		result |= RenderTransformationEntity(
			"RenderScaleEntity", "Scale",
			component, EComponentEntityType::ENTITY_SCALE,
			0.01f, 1E-3f, 10.f
		);
	}
	return result;
}

void ComponentHandler::HandleAnimationPlayer(SkeletalMeshComponent* skeletalMeshComponent)
{
	SeparatorText("Animation Previewer");
	AnimationPlayer* animationPlayer = skeletalMeshComponent->GetAnimationPlayer();
	const AnimationAsset* playingAnimationAsset = animationPlayer->GetPlayingAnimationAsset();
	const string playingAnimationName = playingAnimationAsset ? playingAnimationAsset->GetAssetName() : "Select Animation Asset";

	AssetManager* assetManager = AssetManager::GetInstance();
	const vector<string> animationNames = assetManager->GetAssetNames(EAssetType::ASSET_TYPE_ANIMATION);

	m_animationPreview.SetSelectableItems(playingAnimationName, animationNames);
	
	if (m_animationPreview.Draw())
	{
		animationPlayer->PlayAnimation(assetManager->GetAnimationAsset(m_selectedAnimationName), INFINITE);
	}
}

void ComponentHandler::HandleMeshComponent(AMeshComponent* meshComponent)
{
	// TBD =================================================================================
	SeparatorText("Materials");
	for (const string& materialName : meshComponent->GetModelMaterialName())
	{
		Text(materialName.c_str());
	}
	// =====================================================================================
}

void ComponentHandler::HandleSphereCollisionComponent(SphereCollisionComponent* sphereCollisionComponent)
{
	SeparatorText("Sphere Properties");
	Text("Radius");
	PushID("SphereCollisionComponentRadius");
	if (DragFloat("", &sphereCollisionComponent->Radius, 0.1f, 0.0f))
	{
		sphereCollisionComponent->UpdateEntity();
	}
	PopID();
}

void ComponentHandler::HandleOrientedCollisionComponent(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	SeparatorText("Oriented-Box Properties");
	Text("Extends");
	PushID("OrientedBoxCollisionComponentExtents");
	if (DragFloat3("", &orientedBoxCollisionComponent->Extents.x, 0.1f, 0.0f))
	{
		orientedBoxCollisionComponent->UpdateEntity();
	}
	PopID();
}

void ComponentHandler::HandleLightEntity(LightComponent* lightComponent, const bool& isHandleSpotPower)
{
	constexpr uint8_t lightEntityUpdateOption = GetComponentUpdateOption(ELightComponentUpdateOption::LIGHT_ENTITY);
	bool isChanged = false;

	SeparatorText("Light Entities");
	SLightEntity* lightEntity = lightComponent->GetLightEntityAddress();
	
	PushID("lightPowerEntity");
	Text("Light Power");
	if (DragFloat("", &lightEntity->m_lightPower, 0.01f, 0.f, 10.f))
	{
		isChanged |= true;
	};
	PopID();

	PushID("falloffStartEntity");
	Text("Fall-Off Start Distance");
	if (DragFloat("", &lightEntity->m_fallOffStart, 1.f, 0.f, 1E6))
	{
		isChanged |= true;
	}
	PopID();

	PushID("falloffEndEntity");
	Text("Fall-Off End Distance");
	if (DragFloat("", &lightEntity->m_fallOffEnd, 1.f, lightEntity->m_fallOffStart + 100, 1E6))
	{
		isChanged |= true;
	}
	PopID();

	if (isHandleSpotPower)
	{
		PushID("spotPowerEntity");
		Text("Spot Power");
		if (DragFloat("", &lightEntity->m_spotPower, 0.01f, 0.f, 10.f))
		{
			isChanged |= true;
		}
		PopID();
	}

	if (isChanged) lightComponent->UpdateLightEntity();
}

bool ComponentHandler::RenderTransformationEntity(
	const char* groupID, const char* entityName, 
	AComponent* component, const EComponentEntityType& entityType,
	const float& valueSpeed, const float& minValue, const float& maxValue
)
{
	bool result = false;
	static const XMVECTOR zeroVector = XMVectorZero();
	static const XMVECTOR defaultScale = XMVectorSet(1.f, 1.f, 1.f, 0.f);

	AComponent* parentComponent = component->GetParentComponent();
	XMVECTOR absoluteParentEntity = XMVectorZero();
	XMVECTOR absoluteEntity = XMVectorZero();
	float* relativeEntityAddress = nullptr;

	auto GetParentEntity = [&]() -> const XMVECTOR&
		{
			switch (entityType)
			{
			case EComponentEntityType::ENTITY_POSITION: return parentComponent ? parentComponent->GetAbsolutePosition() : zeroVector;
			case EComponentEntityType::ENTITY_ANGLE: return parentComponent ? parentComponent->GetAbsoluteAngle() : zeroVector;
			case EComponentEntityType::ENTITY_SCALE: return defaultScale;
			default: throw invalid_argument("Invalid entity type");
			}
		};

	auto GetRelativeEntityRef = [&]() -> XMVECTOR&
		{
			switch (entityType) {
			case EComponentEntityType::ENTITY_POSITION: return const_cast<XMVECTOR&>(component->GetLocalPosition());
			case EComponentEntityType::ENTITY_ANGLE: return const_cast<XMVECTOR&>(component->GetLocalAngle());
			case EComponentEntityType::ENTITY_SCALE: return const_cast<XMVECTOR&>(component->GetScale());
			default: throw invalid_argument("Invalid entity type");
			}
		};

	const auto RenderAbsoluteRelativeSelector = [&]() -> void
		{
			if (parentComponent == nullptr)
			{
				switch (entityType)
				{
				case EComponentEntityType::ENTITY_POSITION: IsPositionAbsolute = true; break;
				case EComponentEntityType::ENTITY_ANGLE: IsAngleAbsolute = true; break;
				case EComponentEntityType::ENTITY_SCALE: break;
				default: throw invalid_argument("Invalid entity type");
				}
			}

			switch (entityType) 
			{
			case EComponentEntityType::ENTITY_POSITION: m_absRelativeComboPosition.Draw(); break;
			case EComponentEntityType::ENTITY_ANGLE: m_absRelativeComboAngle.Draw(); break;
			case EComponentEntityType::ENTITY_SCALE: break;
			default: throw invalid_argument("Invalid entity type");
			}
		};

	const auto GetIsAbsolute = [&]() -> const bool
		{
			switch (entityType)
			{
			case EComponentEntityType::ENTITY_POSITION: return IsPositionAbsolute;
			case EComponentEntityType::ENTITY_ANGLE: return IsAngleAbsolute;
			case EComponentEntityType::ENTITY_SCALE: return false;
			default: throw invalid_argument("Invalid entity type");
			}
		};

	const auto EntityHandler = [&](const XMVECTOR& v1, const XMVECTOR& v2) -> XMVECTOR
		{
			switch (entityType)
			{
			case EComponentEntityType::ENTITY_POSITION: return XMVectorAdd(v1, v2);
			case EComponentEntityType::ENTITY_ANGLE: return XMVectorAdd(v1, v2);
			case EComponentEntityType::ENTITY_SCALE: return XMVectorMultiply(v1, v2);
			default: throw invalid_argument("Invalid entity type");
			}
		};

	const auto EntityInverseHandler = [&](const XMVECTOR& v1, const XMVECTOR& v2) -> XMVECTOR
		{
			switch (entityType)
			{
			case EComponentEntityType::ENTITY_POSITION: return XMVectorSubtract(v1, v2);
			case EComponentEntityType::ENTITY_ANGLE: return XMVectorSubtract(v1, v2);
			case EComponentEntityType::ENTITY_SCALE: return XMVectorDivide(v1, v2);
			default: throw invalid_argument("Invalid entity type");
			}
		};

	PushID(groupID);
	Text(entityName);
	RenderAbsoluteRelativeSelector();
	SameLine();

	absoluteParentEntity = GetParentEntity();
	XMVECTOR& relativeEntity = GetRelativeEntityRef();
	absoluteEntity = EntityHandler(absoluteParentEntity, relativeEntity);
	relativeEntityAddress = &relativeEntity.m128_f32[0];

	if (GetIsAbsolute())
	{
		if (DragFloat3("", &absoluteEntity.m128_f32[0], valueSpeed, minValue, maxValue))
		{
			XMVECTOR updatedRelativeEntity = EntityInverseHandler(absoluteEntity, absoluteParentEntity);
			memcpy(relativeEntityAddress, &updatedRelativeEntity.m128_f32[0], sizeof(updatedRelativeEntity));
			component->UpdateEntity();
			result |= true;
		}
	}
	else 
	{
		if (DragFloat3("", relativeEntityAddress, valueSpeed, minValue, maxValue))
		{
			component->UpdateEntity();
			result |= true;
		}
	}

	PopID();
	return result;
}
