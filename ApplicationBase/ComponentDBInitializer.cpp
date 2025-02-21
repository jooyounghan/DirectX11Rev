#include "ComponentDBInitializer.h"

#include "AssetManager.h"
#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"
#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"
#include "SpotLightComponent.h"
#include "PointLightComponent.h"

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
	LoadStaticMeshComponent(staticMeshComponent);
	LoadModelMaterials(staticMeshComponent);

}

void ComponentDBInitializer::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
	LoadSkeletalMeshComponent(skeletalMeshComponent);
	LoadModelMaterials(skeletalMeshComponent);
}

void ComponentDBInitializer::Visit(CameraComponent* cameraComponent)
{

}

void ComponentDBInitializer::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
}

void ComponentDBInitializer::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
}

void ComponentDBInitializer::Visit(SpotLightComponent* spotLightComponent)
{
}

void ComponentDBInitializer::Visit(PointLightComponent* pointLightComponent)
{
}

void ComponentDBInitializer::LoadStaticMeshComponent(StaticMeshComponent* staticMeshComponent)
{
	AssetManager* assetManager = AssetManager::GetInstance();

	const uint32_t componentID = staticMeshComponent->GetComponentID();
	const std::string staticMeshComponentTableName = "static_mesh_components";

	Table staticMeshComponentTable = m_schemaCached->getTable(staticMeshComponentTableName, true);
	RowResult rowResult = staticMeshComponentTable
		.select("static_mesh_name").where("static_mesh_component_id = :static_mesh_component_id")
		.bind("static_mesh_component_id", componentID).lockShared().execute();

	auto row = rowResult.fetchOne();
	if (!row.isNull())
	{
		const std::string static_asset_name = row[0].get<std::string>();
		staticMeshComponent->SetStaticMeshName(static_asset_name);
		staticMeshComponent->UpdateStaticMeshAsset(*assetManager);
	}
}

void ComponentDBInitializer::LoadSkeletalMeshComponent(SkeletalMeshComponent* skeletalMeshComponent)
{
	AssetManager* assetManager = AssetManager::GetInstance();

	const uint32_t componentID = skeletalMeshComponent->GetComponentID();
	const std::string skeletalMeshComponentTableName = "skeletal_mesh_components";

	Table skeletalMeshComponentTable = m_schemaCached->getTable(skeletalMeshComponentTableName, true);
	RowResult rowResult = skeletalMeshComponentTable
		.select("skeletal_mesh_name").where("skeletal_mesh_component_id = :skeletal_mesh_component_id")
		.bind("skeletal_mesh_component_id", componentID).lockShared().execute();

	auto row = rowResult.fetchOne();
	if (!row.isNull())
	{
		const std::string skeletal_asset_name = row[0].get<std::string>();
		skeletalMeshComponent->SetSkeletalMeshName(skeletal_asset_name);
		skeletalMeshComponent->UpdateSkeletalMeshAsset(*assetManager);
	}
}

void ComponentDBInitializer::LoadModelMaterials(AMeshComponent* meshComponent)
{
	AssetManager* assetManager = AssetManager::GetInstance();

	const uint32_t componentID = meshComponent->GetComponentID();
	const std::string meshComponentInformationTableName = "mesh_component_informations";

	Table meshComponentInformationTable = m_schemaCached->getTable(meshComponentInformationTableName, true);
	RowResult rowResult = meshComponentInformationTable
		.select("material_names").where("mesh_component_id = :mesh_component_id")
		.bind("mesh_component_id", componentID).lockShared().execute();


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
