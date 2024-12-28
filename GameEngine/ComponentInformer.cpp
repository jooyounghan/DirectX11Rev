#include "ComponentInformer.h"
#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"
#include <vector>
#include <string>

using namespace std;
using namespace ImGui;
using namespace DirectX;

ComponentInformer::ComponentInformer(AssetManager* assetManager, ComponentManager* componentManager)
	: m_assetManagerCached(assetManager), m_componentManagerCached(componentManager)
{
}

void ComponentInformer::Visit(StaticMeshComponent* staticMeshComponent)
{
	Text("Skeletal Model Component");
	RenderComponentTransformation(staticMeshComponent);
	RenderMeshComponent(staticMeshComponent);
	Button(staticMeshComponent->GetComponentName().c_str());
}

void ComponentInformer::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
	Text("Static Model Component");
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
	Text("Transformation");

	static bool isAbsolutePosition = 0;
	static bool isAbsoluteAngle = 0;
	static bool isAbsoluteScale = 0;

	AComponent* parentComponent = component->GetParentComponent();

	RenderTransformationEntity(
		isAbsolutePosition,
		"RenderPositionEntity", "Position", 
		component, EComponentEntityType::ENTITY_POSITION,
		1.f, -1E9f, 1E9f
	);
	RenderTransformationEntity(
		isAbsoluteAngle,
		"RenderAngleEntity", "Angle",
		component, EComponentEntityType::ENTITY_ANGLE,
		0.1f, -360.f, 360.f
	);
	RenderTransformationEntity(
		isAbsoluteScale,
		"RenderScaleEntity", "Scale",
		component, EComponentEntityType::ENTITY_SCALE,
		0.01f, 1E-3f, 10.f
	);
}

void ComponentInformer::RenderMeshComponent(AMeshComponent* meshComponent)
{
	Text("Materials");
	for (const string& materialName : meshComponent->GetModelMaterialName())
	{
		Text(materialName.c_str());
	}
}

void ComponentInformer::RenderAbsoluteRelativeSelector(bool& isAbsolute, AComponent* component)
{
	vector<std::string> items = { "Absolute", "Relative" };

	AComponent* parentComponent = component->GetParentComponent();
	if (parentComponent == nullptr)
	{
		isAbsolute = true;
		return;
	}

	size_t previewIdx = isAbsolute ? 0 : 1;
	if (BeginCombo("", items[previewIdx].c_str(), ImGuiComboFlags_WidthFitPreview))
	{
		for (size_t idx = 0; idx < items.size(); idx++)
		{
			const bool isSelected = (isAbsolute == (idx == 0));
			if (Selectable(items[idx].c_str(), isSelected))
			{
				isAbsolute = (idx == 0);
			}

			if (isSelected)
				SetItemDefaultFocus();
		}
		EndCombo();
	}
}

void ComponentInformer::RenderTransformationEntity(
	bool& isAbsolute, const char* groupID,
	const char* entityName, AComponent* component, const EComponentEntityType& entityType,
	const float& valueSpeed, const float& minValue, const float& maxValue
)
{
	PushID(groupID);
	Text(entityName);
	RenderAbsoluteRelativeSelector(isAbsolute, component);
	SameLine();

	AComponent* parentComponent = component->GetParentComponent();
	XMVECTOR absoluteParentEntity = XMVectorZero();
	XMVECTOR absoluteEntity = XMVectorZero();
	float* relativeEntityAddress = nullptr;

	const auto getParentEntity = [&]() -> XMVECTOR 
		{
			switch (entityType)	
			{
			case EComponentEntityType::ENTITY_POSITION: return parentComponent ? parentComponent->GetAbsolutePosition() : XMVectorZero();
			case EComponentEntityType::ENTITY_ANGLE: return parentComponent ? parentComponent->GetAbsoluteAngle() : XMVectorZero();
			case EComponentEntityType::ENTITY_SCALE: return parentComponent ? parentComponent->GetAbsoluteScale() : XMVectorZero();
			default: return XMVectorZero();
			}
	};

	const auto getRelativeEntityRef = [&]() -> XMVECTOR& 
		{
			switch (entityType) {
			case EComponentEntityType::ENTITY_POSITION: return component->GetRelativePositionRef();
			case EComponentEntityType::ENTITY_ANGLE: return component->GetRelativeAngleRef();
			case EComponentEntityType::ENTITY_SCALE: return component->GetRelativeScaleRef();
			default: throw std::invalid_argument("Invalid entity type");
			}
	};

	absoluteParentEntity = getParentEntity();
	XMVECTOR& relativeEntity = getRelativeEntityRef();
	absoluteEntity = XMVectorAdd(absoluteParentEntity, relativeEntity);
	relativeEntityAddress = &relativeEntity.m128_f32[0];

	if (isAbsolute) 
	{
		if (DragFloat3("", &absoluteEntity.m128_f32[0], valueSpeed, minValue, maxValue))
		{
			auto subtractor = entityType != EComponentEntityType::ENTITY_ANGLE ? &XMVectorSubtract : &XMVectorSubtractAngles;
			XMVECTOR updatedRelativeEntity = subtractor(absoluteEntity, absoluteParentEntity);
			memcpy(relativeEntityAddress, &updatedRelativeEntity.m128_f32[0], sizeof(updatedRelativeEntity));
			component->UpdateAbsoluteEntities();
			component->SetIsModified(true);
		}
	}
	else 
	{
		if (DragFloat3("", relativeEntityAddress, valueSpeed, minValue, maxValue))
		{
			component->UpdateAbsoluteEntities();
			component->SetIsModified(true);
		}
	}

	PopID();
	Separator();
}
