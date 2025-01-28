#include "ComponentDBRemover.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"

using namespace std;
using namespace mysqlx;

ComponentDBRemover::ComponentDBRemover(Schema* schema)
	: m_schemaCached(schema)
{
}

void ComponentDBRemover::Visit(StaticMeshComponent* staticMeshComponent)
{
	DeleteComponetFromSceneInformation(staticMeshComponent);

	const uint32_t& componentID = staticMeshComponent->GetComponentID();

	const std::string staticMeshComponentTableName = "static_mesh_components";
	Table staticMeshComponentTable = m_schemaCached->getTable(staticMeshComponentTableName, true);
	staticMeshComponentTable.remove().where("static_mesh_component_id = :static_mesh_component_id").bind("static_mesh_component_id", componentID).execute();

	DeleteMeshComponent(staticMeshComponent);
	DeleteComponent(staticMeshComponent);

}

void ComponentDBRemover::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
	DeleteComponetFromSceneInformation(skeletalMeshComponent);

	const uint32_t& componentID = skeletalMeshComponent->GetComponentID();

	const std::string skeletalMeshComponentTableName = "skeletal_mesh_components";
	Table skeletalMeshComponentTable = m_schemaCached->getTable(skeletalMeshComponentTableName, true);
	skeletalMeshComponentTable.remove().where("skeletal_mesh_component_id = :static_mesh_component_id").bind("static_mesh_component_id", componentID).execute();

	DeleteMeshComponent(skeletalMeshComponent);
	DeleteComponent(skeletalMeshComponent);
}

void ComponentDBRemover::Visit(CameraComponent* cameraComponent)
{
	DeleteComponetFromSceneInformation(cameraComponent);

	const uint32_t& componentID = cameraComponent->GetComponentID();

	const std::string cameraComponentTableName = "camera_components";
	Table cameraComponentTable = m_schemaCached->getTable(cameraComponentTableName, true);
	cameraComponentTable.remove().where("camera_component_id = :camera_component_id").bind("camera_component_id", componentID).execute();

	DeleteComponent(cameraComponent);

}

void ComponentDBRemover::Visit(ARenderSphereCollisionComponent* renderSphereCollisionComponent)
{
}

void ComponentDBRemover::Visit(ARenderOrientedBoxCollisionComponent* renderOrientedBoxCollisionComponent)
{
}

void ComponentDBRemover::Visit(ARenderFrustumCollisionComponent* renderFrustumCollisionComponent)
{
}

void ComponentDBRemover::DeleteComponent(AComponent* component)
{
	const uint32_t& componentID = component->GetComponentID();

	const std::string componentTableName = "components";
	Table componentTable = m_schemaCached->getTable(componentTableName, true);
	componentTable.remove().where("component_id = :component_id").bind("component_id", componentID).execute();
}

void ComponentDBRemover::DeleteComponetFromSceneInformation(AComponent* component)
{
	const uint32_t& componentID = component->GetComponentID();

	const std::string sceneInformationTableName = "scene_informations";
	Table sceneInformationTable = m_schemaCached->getTable(sceneInformationTableName, true);
	sceneInformationTable.remove().where("component_id = :component_id").bind("component_id", componentID).execute();

}

void ComponentDBRemover::DeleteMeshComponent(AMeshComponent* meshComponent)
{
	const uint32_t& componentID = meshComponent->GetComponentID();

	const std::string meshComponentInformationTableName = "mesh_component_informations";
	Table meshComponentInformationTable = m_schemaCached->getTable(meshComponentInformationTableName, true);
	meshComponentInformationTable.remove().where("mesh_component_id = :mesh_component_id").bind("mesh_component_id", componentID).execute();
}
