#include "ComponentDBUpdater.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"
#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"

#include "SpotLightComponent.h"
#include "PointLightComponent.h"

#include "nlohmann/json.hpp"

#include <d3d11.h>

using namespace std;
using namespace mysqlx;
using namespace DirectX;

using json = nlohmann::json;

ComponentDBUpdater::ComponentDBUpdater(Schema* schema)
	: m_schemaCached(schema)
{
}

void ComponentDBUpdater::Visit(StaticMeshComponent* staticMeshComponent)
{
	UpdateComponent(staticMeshComponent);
	UpdateStaticMeshComponent(staticMeshComponent);
}

void ComponentDBUpdater::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
	UpdateComponent(skeletalMeshComponent);
	UpdateSkeletalMeshComponent(skeletalMeshComponent);
}

void ComponentDBUpdater::Visit(CameraComponent* cameraComponent)
{
	UpdateComponent(cameraComponent);
	UpdateCameraComponent(cameraComponent);
}

void ComponentDBUpdater::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
	UpdateComponent(sphereCollisionComponent);
	UpdateSphereCollisionComponent(sphereCollisionComponent);
}

void ComponentDBUpdater::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	UpdateComponent(orientedBoxCollisionComponent);
	UpdateOrientedBoxCollisionComponent(orientedBoxCollisionComponent);
}

void ComponentDBUpdater::Visit(SpotLightComponent* spotLightComponent)
{
	UpdateComponent(spotLightComponent);
	UpdateLightComponent(spotLightComponent);
	UpdateSpotLightComponent(spotLightComponent);
}

void ComponentDBUpdater::Visit(PointLightComponent* pointLightComponent)
{
	UpdateComponent(pointLightComponent);
	UpdateLightComponent(pointLightComponent);
}

void ComponentDBUpdater::UpdateComponent(AComponent* component)
{
	const uint32_t& componentID = component->GetComponentID();

	const std::string componentTableName = "components";
	Table componentTable = m_schemaCached->getTable(componentTableName, true);
		
	const XMVECTOR& position = component->GetLocalPosition();
	const XMVECTOR& angle = component->GetLocalAngle();
	const XMVECTOR& scale = component->GetScale();

	componentTable.update()
		.set("position_x", position.m128_f32[0])
		.set("position_y", position.m128_f32[1])
		.set("position_z", position.m128_f32[2])
		.set("angle_x", angle.m128_f32[0])
		.set("angle_y", angle.m128_f32[1])
		.set("angle_z", angle.m128_f32[2])
		.set("scale_x", scale.m128_f32[0])
		.set("scale_y", scale.m128_f32[1])
		.set("scale_z", scale.m128_f32[2])
		.where("component_id = :component_id")
		.bind("component_id", componentID).execute();

}

void ComponentDBUpdater::UpdateMeshComponent(AMeshComponent* meshComponent)
{
	const uint32_t& componentID = meshComponent->GetComponentID();

	const std::string meshComponentInformationTableName = "mesh_component_informations";
	Table meshComponentInformationTable = m_schemaCached->getTable(meshComponentInformationTableName, true);

	const vector<std::string>& materialNames = meshComponent->GetModelMaterialName();

	json jsonObject;
	jsonObject["material_names"] = materialNames;

	std::string jsonString = jsonObject.dump();

	meshComponentInformationTable.update()
		.set("material_names", jsonString)
		.where("mesh_component_id = :component_id")
		.bind("mesh_component_id", componentID).execute();
}

void ComponentDBUpdater::UpdateStaticMeshComponent(StaticMeshComponent* staticMeshComponent)
{
	const uint32_t& componentID = staticMeshComponent->GetComponentID();
	const std::string staticMeshTableName = "static_mesh_components";
	Table staticMeshTable = m_schemaCached->getTable(staticMeshTableName, true);
	staticMeshTable.update()
		.set("static_mesh_name", staticMeshComponent->GetStaticMeshName())
		.where("static_mesh_component_id = :static_mesh_component_id")
		.bind("static_mesh_component_id", componentID).execute();

}

void ComponentDBUpdater::UpdateSkeletalMeshComponent(SkeletalMeshComponent* skeletalMeshComponent)
{
	const uint32_t& componentID = skeletalMeshComponent->GetComponentID();
	const std::string skeletalMeshTableName = "skeletal_mesh_components";
	Table skeletalMeshTable = m_schemaCached->getTable(skeletalMeshTableName, true);
	skeletalMeshTable.update()
		.set("skeletal_mesh_name", skeletalMeshComponent->GetSkeletalMeshName())
		.where("skeletal_mesh_component_id = :skeletal_mesh_component_id")
		.bind("skeletal_mesh_component_id", componentID).execute();
}

void ComponentDBUpdater::UpdateCameraComponent(CameraComponent* cameraComponent)
{
	const uint32_t& componentID = cameraComponent->GetComponentID();
	const std::string cameraTableName = "camera_components";
	const D3D11_VIEWPORT& viewport = cameraComponent->GetViewport();
	Table cameraTable = m_schemaCached->getTable(cameraTableName, true);
	cameraTable.update()
		.set("width", viewport.Width)
		.set("height", viewport.Height)
		.set("fov_angle", cameraComponent->GetFovAngle())
		.set("near_z", cameraComponent->GetNearZ())
		.set("far_z", cameraComponent->GetFarZ())
		.where("camera_component_id = :camera_component_id")
		.bind("camera_component_id", componentID).execute();
}

void ComponentDBUpdater::UpdateSphereCollisionComponent(SphereCollisionComponent* sphereCollisionComponent)
{
	const uint32_t& componentID = sphereCollisionComponent->GetComponentID();
	const std::string sphereCollisionTableName = "sphere_collision_components";
	Table sphereCollisionTable = m_schemaCached->getTable(sphereCollisionTableName, true);
	sphereCollisionTable.update()
		.set("radius", sphereCollisionComponent->Radius)
		.where("sphere_collision_component_id = :sphere_collision_component_id")
		.bind("sphere_collision_component_id", componentID).execute();
}

void ComponentDBUpdater::UpdateOrientedBoxCollisionComponent(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	const uint32_t& componentID = orientedBoxCollisionComponent->GetComponentID();
	const std::string orientedBoxCollisionName = "oriented_box_collision_components";
	Table orientedBoxCollision = m_schemaCached->getTable(orientedBoxCollisionName, true);
	orientedBoxCollision.update()
		.set("extent_x", orientedBoxCollisionComponent->Extents.x)
		.set("extent_y", orientedBoxCollisionComponent->Extents.y)
		.set("extent_z", orientedBoxCollisionComponent->Extents.z)
		.where("oriented_box_collision_component_id = :oriented_box_collision_component_id")
		.bind("oriented_box_collision_component_id", componentID).execute();
}

void ComponentDBUpdater::UpdateLightComponent(LightComponent* ligthComponent)
{
	const uint32_t& componentID = ligthComponent->GetComponentID();

	const std::string lightComponentTableName = "light_components";
	Table lightComponentTable = m_schemaCached->getTable(lightComponentTableName, true);

	SLightEntity* lightEntity = ligthComponent->GetLightEntityAddress();

	lightComponentTable.update()
		.set("light_power", lightEntity->m_lightPower)
		.set("falloff_start", lightEntity->m_fallOffStart)
		.set("falloff_end", lightEntity->m_fallOffEnd)
		.set("spot_power", lightEntity->m_spotPower)
		.where("light_component_id = :light_component_id")
		.bind("light_component_id", componentID).execute();
}

void ComponentDBUpdater::UpdateSpotLightComponent(SpotLightComponent* spotLigthComponent)
{
	const uint32_t& componentID = spotLigthComponent->GetComponentID();

	const std::string spotLightComponentTableName = "spot_light_components";
	Table spotLightComponentTable = m_schemaCached->getTable(spotLightComponentTableName, true);

	const float& fovAngle = spotLigthComponent->GetFovAngle();

	spotLightComponentTable.update()
		.set("fov_angle", fovAngle)
		.where("spot_light_component_id = :spot_light_component_id")
		.bind("spot_light_component_id", componentID).execute();
}
