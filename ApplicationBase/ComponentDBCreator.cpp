#include "ComponentDBCreator.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"

#include "Scene.h"

#include "nlohmann/json.hpp"

using namespace std;
using namespace mysqlx;
using namespace DirectX;

using json = nlohmann::json;

ComponentDBCreator::ComponentDBCreator(Schema* schema)
	: m_schemaCached(schema)
{
}

void ComponentDBCreator::Visit(StaticMeshComponent* staticMeshComponent)
{
	AddMeshComponent(staticMeshComponent);
	AddComponetToType(staticMeshComponent, EComponentType::STATIC_COMPONENT);

	const uint32_t& componentID = staticMeshComponent->GetComponentID();
	const std::string staticMeshTableName = "static_mesh_components";
	Table staticMeshTable = m_schemaCached->getTable(staticMeshTableName, true);
	staticMeshTable.insert("static_mesh_component_id", "static_mesh_name")
		.values(componentID, staticMeshComponent->GetStaticMeshName()).execute();
}

void ComponentDBCreator::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
	AddMeshComponent(skeletalMeshComponent);
	AddComponetToType(skeletalMeshComponent, EComponentType::SKELETAL_COMPONENT);

	const uint32_t& componentID = skeletalMeshComponent->GetComponentID();
	const std::string skeletalMeshTableName = "skeletal_mesh_components";
	Table skeletalMeshTable = m_schemaCached->getTable(skeletalMeshTableName, true);
	skeletalMeshTable.insert("skeletal_mesh_component_id", "skeletal_mesh_name")
		.values(componentID, skeletalMeshComponent->GetSkeletalMeshName()).execute();
}

void ComponentDBCreator::Visit(CameraComponent* cameraComponent)
{
	AddComponetToType(cameraComponent, EComponentType::CAMERA_COMPONENT);

	const D3D11_VIEWPORT& viewport = cameraComponent->GetViewport();

	const uint32_t& componentID = cameraComponent->GetComponentID();
	const std::string cameraTableName = "camera_components";
	Table cameraTable = m_schemaCached->getTable(cameraTableName, true);
	cameraTable.insert("camera_component_id", "width", "height", "near_z", "far_z", "fov_angle")
		.values(componentID, viewport.Width, viewport.Height,
			cameraComponent->GetNearZ(), cameraComponent->GetFarZ(),
			cameraComponent->GetFovAngle()
		);
}

void ComponentDBCreator::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
	// TBD
}

void ComponentDBCreator::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	// TBD
}

void ComponentDBCreator::Visit(SpotLightComponent* spotLightComponent)
{
	// TBD
}

void ComponentDBCreator::Visit(PointLightComponent* pointLightComponent)
{
	// TBD
}

void ComponentDBCreator::AddScene(Scene* scene)
{
	const std::string sceneTableName = "scenes";

	Table sceneTable = m_schemaCached->getTable(sceneTableName, true);
	sceneTable.insert("scene_id", "scene_description", "scene_static_mesh_name", "scene_ibl_material_name")
		.values(scene->GetSceneID(), scene->GetSceneDescription(), scene->GetSceneStaticMeshName(), scene->GetSceneIBLMaterialName())
		.execute();
}

void ComponentDBCreator::AddComponent(Scene* scene, AComponent* parentComponent, AComponent* component)
{
	const uint32_t& componentID = component->GetComponentID();

	if (scene != nullptr)
	{
		const std::string sceneInformationTableName = "scene_informations";
		Table sceneInformationTable = m_schemaCached->getTable(sceneInformationTableName, true);

		sceneInformationTable.insert("scene_id", "component_id").values(scene->GetSceneID(), componentID).execute();
	}

	uint32_t parentComponentID = parentComponent != nullptr ? parentComponent->GetComponentID() : NULL;

	const std::string componentsTableName = "components";
	Table componentsTable = m_schemaCached->getTable(componentsTableName, true);

	const XMVECTOR& position = component->GetLocalPosition();
	const XMVECTOR& angle = component->GetLocalAngle();
	const XMVECTOR& scale = component->GetScale();

	componentsTable.insert("component_id", "parent_component_id", "component_name", 
		"position_x", "position_y", "position_z",
		"angle_x", "angle_y", "angle_z",
		"scale_x", "scale_y", "scale_z"
	).values(component->GetComponentID(), parentComponentID, component->GetComponentName(),
		position.m128_f32[0], position.m128_f32[1], position.m128_f32[2],
		angle.m128_f32[0], angle.m128_f32[1], angle.m128_f32[2],
		scale.m128_f32[0], scale.m128_f32[1], scale.m128_f32[2]
	).execute();
}

void ComponentDBCreator::AddMeshComponent(AMeshComponent* meshComponent)
{
	const uint32_t& componentID = meshComponent->GetComponentID();
	const vector<std::string>& materialNames = meshComponent->GetModelMaterialName();

	json jsonObject;
	jsonObject["material_names"] = materialNames;

	const std::string meshComponentInformationTableName = "mesh_component_informations";
	Table meshComponentInformationTable = m_schemaCached->getTable(meshComponentInformationTableName, true);
	meshComponentInformationTable.insert("mesh_component_id", "material_names").values(componentID, jsonObject.dump()).execute();
}

void ComponentDBCreator::AddComponetToType(AComponent* component, const EComponentType& componentType)
{
	const uint32_t& componentID = component->GetComponentID();

	const std::string componentsToTypeTableName = "components_to_type";
	Table componentsToTypeTable = m_schemaCached->getTable(componentsToTypeTableName, true);
	componentsToTypeTable.insert("component_id", "component_type")
		.values(componentID, static_cast<uint32_t>(componentType)).execute();
}

