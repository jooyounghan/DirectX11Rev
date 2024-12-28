#include "ComponentUpdater.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"

#include "nlohmann/json.hpp"

using namespace std;
using namespace mysqlx;
using namespace DirectX;

using json = nlohmann::json;

ComponentUpdater::ComponentUpdater(Schema* schema)
	: m_schemaCached(schema)
{
}

void ComponentUpdater::Visit(StaticMeshComponent* staticMeshComponent)
{
	UpdateComponent(staticMeshComponent);

	const uint32_t& componentID = staticMeshComponent->GetComponentID();
	const std::string staticMeshTableName = "static_mesh_components";
	Table staticMeshTable = m_schemaCached->getTable(staticMeshTableName, true);
	staticMeshTable.update()
		.set("static_mesh_name", staticMeshComponent->GetStaticMeshName())
		.where("static_mesh_component_id = :static_mesh_component_id")
		.bind("static_mesh_component_id", componentID).execute();
}

void ComponentUpdater::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
	UpdateComponent(skeletalMeshComponent);

	const uint32_t& componentID = skeletalMeshComponent->GetComponentID();
	const std::string skeletalMeshTableName = "skeletal_mesh_components";
	Table skeletalMeshTable = m_schemaCached->getTable(skeletalMeshTableName, true);
	skeletalMeshTable.update()
		.set("skeletal_mesh_name", skeletalMeshComponent->GetSkeletalMeshName())
		.where("skeletal_mesh_component_id = :skeletal_mesh_component_id")
		.bind("skeletal_mesh_component_id", componentID).execute();
}

void ComponentUpdater::Visit(CameraComponent* cameraComponent)
{
	UpdateComponent(cameraComponent);

	const uint32_t& componentID = cameraComponent->GetComponentID();
	const std::string cameraTableName = "camera_components";
	Table cameraTable = m_schemaCached->getTable(cameraTableName, true);
	cameraTable.update()
		.set("width", cameraComponent->Width)
		.set("height", cameraComponent->Height)
		.set("near_z", cameraComponent->GetNearZ())
		.set("far_z", cameraComponent->GetFarZ())
		.set("fov_angle", cameraComponent->GetFovAngle())
		.where("camera_component_id = :camera_component_id")
		.bind("camera_component_id", componentID).execute();
}

void ComponentUpdater::UpdateComponent(AComponent* component)
{
	const uint32_t& componentID = component->GetComponentID();

	const std::string componentTableName = "components";
	Table componentTable = m_schemaCached->getTable(componentTableName, true);
		
	const XMVECTOR& position = component->GetRelativePositionRef();
	const XMVECTOR& angle = component->GetRelativeAngleRef();
	const XMVECTOR& scale = component->GetRelativeScaleRef();

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

void ComponentUpdater::UpdateMeshComponent(AMeshComponent* meshComponent)
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
