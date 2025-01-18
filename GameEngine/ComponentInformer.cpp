#include "ComponentInformer.h"

#include "AssetManager.h"
#include "ComponentManager.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"

#include <vector>
#include <string>

using namespace std;
using namespace ImGui;
using namespace DirectX;

bool ComponentInformer::isPositionAbsolute = true;
bool ComponentInformer::isAngleAbsolute = true;
bool ComponentInformer::isScaleAbsolute = true;

ComponentInformer::ComponentInformer(AssetManager* assetManager, ComponentManager* componentManager)
	: m_assetManagerCached(assetManager), m_componentManagerCached(componentManager),
	m_absRelativeComboPosition("AbsRelativePositionCombo", "", ImGuiComboFlags_WidthFitPreview),
	m_absRelativeComboAngle("AbsRelativeAngleCombo", "", ImGuiComboFlags_WidthFitPreview),
	m_absRelativeComboScale("AbsRelativeScaleCombo", "", ImGuiComboFlags_WidthFitPreview)
{
	const vector<string> absRelativeStrings = { "Absolute", "Relative" };

	m_absRelativeComboPosition.SetSelectableItems(isPositionAbsolute ? absRelativeStrings[0] : absRelativeStrings[1], absRelativeStrings);
	m_absRelativeComboAngle.SetSelectableItems(isPositionAbsolute ? absRelativeStrings[0] : absRelativeStrings[1], absRelativeStrings);
	m_absRelativeComboScale.SetSelectableItems(isPositionAbsolute ? absRelativeStrings[0] : absRelativeStrings[1], absRelativeStrings);

	m_absRelativeComboPosition.OnSelChanged = [&](const size_t& idx, const string&) { isPositionAbsolute = !static_cast<bool>(idx); };
	m_absRelativeComboAngle.OnSelChanged = [&](const size_t& idx, const string&) { isAngleAbsolute = !static_cast<bool>(idx); };
	m_absRelativeComboScale.OnSelChanged = [&](const size_t& idx, const string&) { isScaleAbsolute = !static_cast<bool>(idx); };
}

void ComponentInformer::Visit(StaticMeshComponent* staticMeshComponent)
{
	Text("Static Model Component");
	RenderComponentTransformation(staticMeshComponent);
	RenderMeshComponent(staticMeshComponent);
	Button(staticMeshComponent->GetComponentName().c_str());
}

void ComponentInformer::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
	Text("Skeletal Model Component");
	RenderComponentTransformation(skeletalMeshComponent);
	RenderMeshComponent(skeletalMeshComponent);
	Button(skeletalMeshComponent->GetComponentName().c_str());
}

void ComponentInformer::Visit(CameraComponent* cameraComponent)
{
	Text("Camera Model Component");
	RenderComponentTransformation(cameraComponent);

	Button(cameraComponent->GetComponentName().c_str());
}


void ComponentInformer::RenderComponentTransformation(AComponent* component)
{
	SeparatorText("Transformation");

	static bool isAbsolutePosition = false;
	static bool isAbsoluteAngle = 0;
	static bool isAbsoluteScale = 0;

	AComponent* parentComponent = component->GetParentComponent();

	RenderTransformationEntity(
		"RenderPositionEntity", "Position", 
		component, EComponentEntityType::ENTITY_POSITION,
		1.f, -1E9f, 1E9f
	);
	RenderTransformationEntity(
		"RenderAngleEntity", "Angle",
		component, EComponentEntityType::ENTITY_ANGLE,
		0.1f, -360.f, 360.f
	);
	RenderTransformationEntity(
		"RenderScaleEntity", "Scale",
		component, EComponentEntityType::ENTITY_SCALE,
		0.01f, 1E-3f, 10.f
	);
}

void ComponentInformer::RenderMeshComponent(AMeshComponent* meshComponent)
{
	SeparatorText("Materials");
	for (const string& materialName : meshComponent->GetModelMaterialName())
	{
		Text(materialName.c_str());
	}
}

void ComponentInformer::RenderTransformationEntity(
	const char* groupID, const char* entityName, 
	AComponent* component, const EComponentEntityType& entityType,
	const float& valueSpeed, const float& minValue, const float& maxValue
)
{
	AComponent* parentComponent = component->GetParentComponent();
	XMVECTOR absoluteParentEntity = XMVectorZero();
	XMVECTOR absoluteEntity = XMVectorZero();
	float* relativeEntityAddress = nullptr;

	const auto GetParentEntity = [&]() -> XMVECTOR
		{
			switch (entityType)
			{
			case EComponentEntityType::ENTITY_POSITION: return parentComponent ? parentComponent->GetAbsolutePosition() : XMVectorZero();
			case EComponentEntityType::ENTITY_ANGLE: return parentComponent ? parentComponent->GetAbsoluteAngle() : XMVectorZero();
			case EComponentEntityType::ENTITY_SCALE: return parentComponent ? parentComponent->GetAbsoluteScale() : XMVectorSet(1.f, 1.f, 1.f, 0.f);
			default: throw std::invalid_argument("Invalid entity type");
			}
		};

	const auto GetRelativeEntityRef = [&]() -> XMVECTOR&
		{
			switch (entityType) {
			case EComponentEntityType::ENTITY_POSITION: return const_cast<XMVECTOR&>(component->GetLocalPosition());
			case EComponentEntityType::ENTITY_ANGLE: return const_cast<XMVECTOR&>(component->GetLocalAngle());
			case EComponentEntityType::ENTITY_SCALE: return const_cast<XMVECTOR&>(component->GetLocalScale());
			default: throw std::invalid_argument("Invalid entity type");
			}
		};

	const auto RenderAbsoluteRelativeSelector = [&]()
		{
			if (parentComponent == nullptr)
			{
				switch (entityType)
				{
				case EComponentEntityType::ENTITY_POSITION: isPositionAbsolute = true; break;
				case EComponentEntityType::ENTITY_ANGLE: isAngleAbsolute = true; break;
				case EComponentEntityType::ENTITY_SCALE: isScaleAbsolute = true; break;
				default: throw std::invalid_argument("Invalid entity type");
				}
				return;
			}

			switch (entityType) 
			{
			case EComponentEntityType::ENTITY_POSITION: return m_absRelativeComboPosition.Draw();
			case EComponentEntityType::ENTITY_ANGLE: return m_absRelativeComboAngle.Draw();
			case EComponentEntityType::ENTITY_SCALE: return m_absRelativeComboScale.Draw();
			default: throw std::invalid_argument("Invalid entity type");
			}
		};

	const auto GetIsAbsolute = [&]() -> const bool
		{
			switch (entityType)
			{
			case EComponentEntityType::ENTITY_POSITION: return isPositionAbsolute;
			case EComponentEntityType::ENTITY_ANGLE: return isAngleAbsolute;
			case EComponentEntityType::ENTITY_SCALE: return isScaleAbsolute;
			default: throw std::invalid_argument("Invalid entity type");
			}
		};

	const auto EntityHandler = [&](const XMVECTOR& v1, const XMVECTOR& v2) -> XMVECTOR
		{
			switch (entityType)
			{
			case EComponentEntityType::ENTITY_POSITION: return XMVectorAdd(v1, v2);
			case EComponentEntityType::ENTITY_ANGLE: return XMVectorAdd(v1, v2);
			case EComponentEntityType::ENTITY_SCALE: return XMVectorMultiply(v1, v2);
			default: throw std::invalid_argument("Invalid entity type");
			}
		};

	const auto EntityInverseHandler = [&](const XMVECTOR& v1, const XMVECTOR& v2) -> XMVECTOR
		{
			switch (entityType)
			{
			case EComponentEntityType::ENTITY_POSITION: return XMVectorSubtract(v1, v2);
			case EComponentEntityType::ENTITY_ANGLE: return XMVectorSubtract(v1, v2);
			case EComponentEntityType::ENTITY_SCALE: return XMVectorDivide(v1, v2);
			default: throw std::invalid_argument("Invalid entity type");
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
			component->SetIsModified(true);
		}
	}
	else 
	{
		if (DragFloat3("", relativeEntityAddress, valueSpeed, minValue, maxValue))
		{
			component->SetIsModified(true);
		}
	}

	PopID();
}
