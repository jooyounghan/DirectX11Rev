#include "ComponentInitializer.h"

#include "Scene.h"
#include "StaticModelComponent.h"
#include "SkeletalModelComponent.h"
#include "CameraComponent.h"

using namespace std;
using namespace mysqlx;

ComponentInitializer::ComponentInitializer(
	AssetManager* assetManager,
	ID3D11Device** deviceAdress,
	mysqlx::Schema* schema
)
	: m_assetManagerCached(assetManager), m_deviceAdressCached(deviceAdress), m_schemaCached(schema)
{
}

void ComponentInitializer::Visit(StaticModelComponent* staticModelComponent)
{
	try
	{
		const uint32_t comopnentID = staticModelComponent->GetComponentID();
		const std::string staticMeshComponentTableName = "static_mesh_components";

		Table staticMeshComponentTable = m_schemaCached->getTable(staticMeshComponentTableName, true);
		RowResult rowResult = staticMeshComponentTable
			.select("static_mesh_name").where("static_mesh_component_id = :static_mesh_component_id")
			.bind("static_mesh_component_id", comopnentID).lockShared().execute();

		auto row = rowResult.fetchOne();
		if (!row.isNull()) 
		{
			const std::string static_asset_name = row[0].get<std::string>();
			staticModelComponent->SetStaticMeshAsset(m_assetManagerCached->GetStaticMeshAsset(static_asset_name));
		}
		staticModelComponent->InitEntity(*m_deviceAdressCached);
	}
	catch (const std::exception& ex)
	{
		OnErrorOccurs(ex.what());
	}
}

void ComponentInitializer::Visit(SkeletalModelComponent* skeletalModelComponent)
{
	try
	{
		const uint32_t comopnentID = skeletalModelComponent->GetComponentID();
		const std::string skeletalMeshComponentTableName = "skeletal_mesh_components";

		Table skeletalMeshComponentTable = m_schemaCached->getTable(skeletalMeshComponentTableName, true);
		RowResult rowResult = skeletalMeshComponentTable
			.select("skeletal_mesh_name").where("skeletal_mesh_component_id = :skeletal_mesh_component_id")
			.bind("skeletal_mesh_component_id", comopnentID).lockShared().execute();

		auto row = rowResult.fetchOne();
		if (!row.isNull())
		{
			const std::string skeletal_asset_name = row[0].get<std::string>();
			skeletalModelComponent->SetSkeletalMeshAsset(m_assetManagerCached->GetSkeletalMeshAsset(skeletal_asset_name));
		}
		skeletalModelComponent->InitEntity(*m_deviceAdressCached);
	}
	catch (const std::exception& ex)
	{
		OnErrorOccurs(ex.what());
	}
}

void ComponentInitializer::Visit(CameraComponent* cameraComponent)
{
	try
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
			const uint32_t height = row[0].get<uint32_t>();
			const float near_z = row[0].get<float>();
			const float far_z = row[0].get<float>();
			const float fov_angle = row[0].get<float>();
			cameraComponent->SetCameraProperties(width, height, near_z, far_z, fov_angle);
		}
		cameraComponent->InitEntity(*m_deviceAdressCached);
	}
	catch (const std::exception& ex)
	{
		OnErrorOccurs(ex.what());
	}
}

void ComponentInitializer::Visit(Scene* scene)
{
	scene->UpdateSceneMeshAsset(*m_assetManagerCached);
	scene->UpdateSceneIBLMaterialAsset(*m_assetManagerCached);
}
