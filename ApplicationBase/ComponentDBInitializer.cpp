#include "ComponentDBInitializer.h"

#include "AssetManager.h"
#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"
#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"

#include "ComponentType.h"
#include "RenderControlOption.h"

#include "nlohmann/json.hpp"

#include <sstream>

using namespace std;
using namespace mysqlx;
using namespace DirectX;

using json = nlohmann::json;

ComponentDBInitializer::ComponentDBInitializer(mysqlx::Schema* schema)
	: m_schemaCached(schema)
{
}

void ComponentDBInitializer::Visit(StaticMeshComponent* staticMeshComponent)
{
	AssetManager* assetManager = AssetManager::GetInstance();

	const uint32_t comopnentID = staticMeshComponent->GetComponentID();
	const std::string staticMeshComponentTableName = "static_mesh_components";

	Table staticMeshComponentTable = m_schemaCached->getTable(staticMeshComponentTableName, true);
	RowResult rowResult = staticMeshComponentTable
		.select("static_mesh_name").where("static_mesh_component_id = :static_mesh_component_id")
		.bind("static_mesh_component_id", comopnentID).lockShared().execute();

	auto row = rowResult.fetchOne();
	if (!row.isNull()) 
	{
		const std::string static_asset_name = row[0].get<std::string>();
		staticMeshComponent->SetStaticMeshName(static_asset_name);
		staticMeshComponent->UpdateStaticMeshAsset(*assetManager);
	}

	LoadModelMaterials(staticMeshComponent);

}

void ComponentDBInitializer::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
	AssetManager* assetManager = AssetManager::GetInstance();

	const uint32_t comopnentID = skeletalMeshComponent->GetComponentID();
	const std::string skeletalMeshComponentTableName = "skeletal_mesh_components";

	Table skeletalMeshComponentTable = m_schemaCached->getTable(skeletalMeshComponentTableName, true);
	RowResult rowResult = skeletalMeshComponentTable
		.select("skeletal_mesh_name").where("skeletal_mesh_component_id = :skeletal_mesh_component_id")
		.bind("skeletal_mesh_component_id", comopnentID).lockShared().execute();

	auto row = rowResult.fetchOne();
	if (!row.isNull())
	{
		const std::string skeletal_asset_name = row[0].get<std::string>();
		skeletalMeshComponent->SetSkeletalMeshName(skeletal_asset_name);
		skeletalMeshComponent->UpdateSkeletalMeshAsset(*assetManager);
	}

	LoadModelMaterials(skeletalMeshComponent);
}

void ComponentDBInitializer::Visit(CameraComponent* cameraComponent)
{
	const uint32_t comopnentID = cameraComponent->GetComponentID();
	const std::string cameraComponentTableName = "camera_components";

	Table cameraComponentTable = m_schemaCached->getTable(cameraComponentTableName, true);
	RowResult rowResult = cameraComponentTable
		.select("width", "height", "near_z", "far_z", "fov_angle").where("camera_component_id = :camera_component_id")
		.bind("camera_component_id", comopnentID).lockShared().execute();

	auto row = rowResult.fetchOne();
	if (!row.isNull())
	{
		const uint32_t width = row[0].get<uint32_t>();
		const uint32_t height = row[1].get<uint32_t>();
		const float near_z = row[2].get<float>();
		const float far_z = row[3].get<float>();
		const float fov_angle = row[4].get<float>();
		cameraComponent->SetCameraProperties(width, height, near_z, far_z, fov_angle);
	}
}

void ComponentDBInitializer::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
	const uint32_t comopnentID = sphereCollisionComponent->GetComponentID();
	const std::string sphereCollisionComponentTableName = "sphere_collision_components";

	Table sphereCollisionComponentTable = m_schemaCached->getTable(sphereCollisionComponentTableName, true);
	RowResult rowResult = sphereCollisionComponentTable
		.select("radius", "collision_option_id").where("sphere_collision_component_id = :sphere_collision_component_id")
		.bind("sphere_collision_component_id", comopnentID).lockShared().execute();

	auto row = rowResult.fetchOne();
	if (!row.isNull())
	{
		const float radius = row[0].get<float>();
		const uint32_t collisionOptionID = row[1].get<uint32_t>();

		const XMVECTOR& absolutePosition = sphereCollisionComponent->GetAbsolutePosition();
		sphereCollisionComponent->SetBoundingProperties(absolutePosition, radius);
		ICollisionOption* collisionOption = CreateCollisionOption(collisionOptionID);
		sphereCollisionComponent->SetCollisionOption(collisionOption);
	}

}

void ComponentDBInitializer::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	const uint32_t comopnentID = orientedBoxCollisionComponent->GetComponentID();
	const std::string orientedBoxCollisionComponentTableName = "oriented_box_collision_components";

	Table orientedBoxCollisionComponentTable = m_schemaCached->getTable(orientedBoxCollisionComponentTableName, true);
	RowResult rowResult = orientedBoxCollisionComponentTable
		.select("extent_x", "extent_y", "extent_z", "collision_option_id").where("oriented_box_collision_component_id = :oriented_box_collision_component_id")
		.bind("oriented_box_collision_component_id", comopnentID).lockShared().execute();

	auto row = rowResult.fetchOne();
	if (!row.isNull())
	{
		const float extendX = row[0].get<float>();
		const float extendY = row[1].get<float>();
		const float extendZ = row[2].get<float>();
		const uint32_t collisionOptionID = row[3].get<uint32_t>();

		const XMVECTOR& absolutePosition = orientedBoxCollisionComponent->GetAbsolutePosition();
		const XMVECTOR& rotationQuaternion =  orientedBoxCollisionComponent->GetAbsoluteRotationQuaternion();
		orientedBoxCollisionComponent->SetBoundingProperties(absolutePosition, XMFLOAT3(extendX, extendY, extendZ), rotationQuaternion);

		ICollisionOption* collisionOption = CreateCollisionOption(collisionOptionID);
		orientedBoxCollisionComponent->SetCollisionOption(collisionOption);
	}
}

void ComponentDBInitializer::LoadModelMaterials(AMeshComponent* meshComponent)
{
	AssetManager* assetManager = AssetManager::GetInstance();

	const uint32_t comopnentID = meshComponent->GetComponentID();
	const std::string meshComponentInformationTableName = "mesh_component_informations";

	Table meshComponentInformationTable = m_schemaCached->getTable(meshComponentInformationTableName, true);
	RowResult rowResult = meshComponentInformationTable
		.select("material_names").where("mesh_component_id = :mesh_component_id")
		.bind("mesh_component_id", comopnentID).lockShared().execute();


	auto row = rowResult.fetchOne();
	if (!row.isNull())
	{
		DbDoc materialNamesDoc = row[0].get<DbDoc>();

		std::stringstream ss;
		materialNamesDoc.print(ss);
		json parsedMaterialNamesJson = json::parse(ss.str());
		vector<std::string> materialNames;

		if (parsedMaterialNamesJson.contains("material_names") && parsedMaterialNamesJson["material_names"].is_array()) 
		{
			for (const auto& materialName : parsedMaterialNamesJson["material_names"])
			{
				materialNames.push_back(materialName.get<std::string>());
			}
		}
		meshComponent->SetModelMaterialName(materialNames);
		meshComponent->UpdateModelMaterial(*assetManager);
	}

}

ICollisionOption* ComponentDBInitializer::CreateCollisionOption(const uint32_t& collitionOptionID)
{
	const ECollisionOptionType collisionType = static_cast<ECollisionOptionType>(collitionOptionID);
	switch (collisionType)
	{
	case ECollisionOptionType::RENDER_CONTROL:
		return new RenderControlOption();
		break;
	}
	return nullptr;
}
