#include "ComponentInitializer.h"

#include "AssetManager.h"
#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"

#include "nlohmann/json.hpp"

#include <sstream>

using namespace std;
using namespace mysqlx;

using json = nlohmann::json;

ComponentInitializer::ComponentInitializer(
	AssetManager* assetManager,
	ID3D11Device* const* deviceAdress,
	mysqlx::Schema* schema
)
	: m_assetManagerCached(assetManager), m_deviceAdressCached(deviceAdress), m_schemaCached(schema)
{
}

void ComponentInitializer::Visit(StaticMeshComponent* staticMeshComponent)
{
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
		staticMeshComponent->UpdateModelMaterial(*m_assetManagerCached);
	}

	LoadModelMaterials(staticMeshComponent);
	staticMeshComponent->InitEntity(*m_deviceAdressCached);

}

void ComponentInitializer::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
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
		skeletalMeshComponent->UpdateSkeletalMeshAsset(*m_assetManagerCached);
	}

	LoadModelMaterials(skeletalMeshComponent);
	skeletalMeshComponent->InitEntity(*m_deviceAdressCached);
}

void ComponentInitializer::Visit(CameraComponent* cameraComponent)
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
	cameraComponent->InitEntity(*m_deviceAdressCached);
}

void ComponentInitializer::LoadModelMaterials(AMeshComponent* meshComponent)
{
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
		meshComponent->UpdateModelMaterial(*m_assetManagerCached);
	}

}
