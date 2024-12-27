#include "ComponentManager.h"

#include "Scene.h"

#include "StaticModelComponent.h"
#include "SkeletalModelComponent.h"
#include "CameraComponent.h"

using namespace std;
using namespace mysqlx;
using namespace DirectX;

ComponentManager::ComponentManager(SessionManager* sessionManager)
	: SchemaManager(sessionManager, "component_db")
{

}

ComponentManager::~ComponentManager()
{
	for (auto& componentIDToComponent : m_componentIDsToComponent)
	{
		delete componentIDToComponent.second;
	}
	for (auto& sceneIDToScene : m_sceneIDsToScene)
	{
		delete sceneIDToScene.second;
	}
}

void ComponentManager::InitComponentManager()
{
	LoadComponentMakers();
	LoadComponents();
	LoadScenes();
	LoadScenesInformation();
}

void ComponentManager::LoadComponentMakers()
{
	try
	{
		const std::string& componentsTypeTableName = "component_types";

		Table componentsTypeTable = getTable(componentsTypeTableName, true);
		RowResult componentTypeResults = componentsTypeTable.select("*").lockShared().execute();

		Row componentTypeResult;
		while ((componentTypeResult = componentTypeResults.fetchOne()))
		{
			const EComponentType componentType = static_cast<EComponentType>(componentTypeResult[0].get<uint32_t>());
			const std::string typeDescription = componentTypeResult[1].get<std::string>();

			switch (componentType)
			{
			case EComponentType::STATIC_COMPONENT:
				m_componentTypesToMaker.emplace(componentType, bind(
					[&](const ComponentID& componentID, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT3& scale)
					{ return new StaticModelComponent(componentID, position, angle, scale); }, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4)
				);
				break;
			case EComponentType::SKELETAL_COMPONENT:
				m_componentTypesToMaker.emplace(componentType, bind(
					[&](const ComponentID& componentID, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT3& scale)
					{ return new SkeletalModelComponent(componentID, position, angle, scale); }, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4)
				);
				break;
			case EComponentType::CAMERA_COMPONENT:
				m_componentTypesToMaker.emplace(componentType, bind(
					[&](const ComponentID& componentID, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT3& scale)
					{ return new CameraComponent(componentID, position, angle, scale); }, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4)
				);
				break;
			}
		}
	}
	catch (const std::exception& ex)
	{
		OnErrorOccurs(ex.what());
	}
}

void ComponentManager::LoadComponents()
{
	try
	{
		const std::string& componentsTableName = "components";
		Table componentsTable = getTable(componentsTableName, true);

		RowResult componentResults = componentsTable.select("component_id", "component_type", "position_x", "position_y", "position_z", "angle_x", "angle_y", "angle_z", "scale_x", "scale_y", "scale_z").where("parent_component_id is NULL").lockShared().execute();
		LoadParentComponentsRecursiveImpl(componentResults, nullptr, componentsTable);

	}
	catch (const std::exception& ex)
	{
		OnErrorOccurs(ex.what());
	}
}

void ComponentManager::LoadScenes()
{
	try
	{
		const std::string& scenesTableName = "scenes";
		Table scenesTable = getTable(scenesTableName, true);

		RowResult sceneResults = scenesTable.select("*").lockShared().execute();

		Row sceneResult;
		while ((sceneResult = sceneResults.fetchOne()))
		{
			const SceneID sceneID = sceneResult[0].get<SceneID>();
			const std::string sceneDescription = sceneResult[1].get<std::string>();

			Scene* scene = new Scene();
			m_sceneIDsToScene.emplace(sceneID, scene);
			m_scenesToDescription.emplace(scene, sceneDescription);
		}

	}
	catch (const std::exception& ex)
	{
		OnErrorOccurs(ex.what());
	}
}

void ComponentManager::LoadScenesInformation()
{
	try
	{
		const std::string& scenesInformationsTableName = "scene_informations";
		Table scenesInformationsTable = getTable(scenesInformationsTableName, true);

		RowResult sceneInformationResults = scenesInformationsTable.select("scene_id", "component_id").lockShared().execute();

		Row sceneInformationResult;
		while ((sceneInformationResult = sceneInformationResults.fetchOne()))
		{
			const SceneID sceneID = sceneInformationResult[0].get<SceneID>();
			const ComponentID componentID = sceneInformationResult[1].get<ComponentID>();

			if (m_sceneIDsToScene.find(sceneID) != m_sceneIDsToScene.end() && m_componentIDsToComponent.find(componentID) != m_componentIDsToComponent.end())
			{
				m_sceneIDsToScene[sceneID]->AddRootComponent(m_componentIDsToComponent[componentID]);
			}
			else
			{
				throw std::exception(format("Scene ID({}) or Component ID({}) Not Founded", sceneID, componentID).c_str());
			}
		}
	}
	catch (const std::exception& ex)
	{
		OnErrorOccurs(ex.what());
	}
}

void ComponentManager::LoadParentComponentsRecursive(const vector<ComponentID>& addedComponentIDs, Table& table)
{
	for (const ComponentID& addedComponentID : addedComponentIDs)
	{
		AComponent* parentComponent = nullptr;
		if (m_componentIDsToComponent.find(addedComponentID) != m_componentIDsToComponent.end())
		{
			parentComponent = m_componentIDsToComponent[addedComponentID];
		}

		RowResult componentResults = table.select("component_id", "component_type", "position_x", "position_y", "position_z", "angle_x", "angle_y", "angle_z", "scale_x", "scale_y", "scale_z")
			.where("parent_component_id = :parent_component_id").bind("parent_component_id", addedComponentID).lockShared().execute();
		LoadParentComponentsRecursiveImpl(componentResults, parentComponent, table);
	}
}

void ComponentManager::LoadParentComponentsRecursiveImpl(RowResult& rowResults, AComponent* parentComponent, Table& table)
{
	Row rowResult;
	vector<ComponentID> addedComponentIDs;
	while ((rowResult = rowResults.fetchOne()))
	{
		const ComponentID componentID = rowResult[0].get<ComponentID>();
		const EComponentType componentType = static_cast<EComponentType>(rowResult[1].get<uint32_t>());

		XMFLOAT3 position = XMFLOAT3(rowResult[2].get<float>(), rowResult[3].get<float>(), rowResult[4].get<float>());
		XMFLOAT3 angle = XMFLOAT3(rowResult[5].get<float>(), rowResult[6].get<float>(), rowResult[7].get<float>());
		XMFLOAT3 scale = XMFLOAT3(rowResult[8].get<float>(), rowResult[9].get<float>(), rowResult[10].get<float>());

		AddComponent(componentID, componentType, position, angle, scale, parentComponent);
		addedComponentIDs.emplace_back(componentID);
	}

	LoadParentComponentsRecursive(addedComponentIDs, table);
}


void ComponentManager::AddComponent(
	const ComponentID& componentID, const EComponentType& componentType, 
	const XMFLOAT3 position, const XMFLOAT3 angle, const XMFLOAT3 scale, 
	AComponent* parentComponent
)
{
	if (m_componentTypesToMaker.find(componentType) != m_componentTypesToMaker.end())
	{
		AComponent* addedComponent = m_componentTypesToMaker[componentType](componentID, position, angle, scale);
		m_componentIDsToComponent.emplace(componentID, addedComponent);
		if (parentComponent != nullptr) parentComponent->AddChildComponent(addedComponent);
	}
}



//
//void ComponentSchema::AddComponent(const EComponentType& componentType, AComponent* parentComponent)
//{
//	const std::string schemaName = getName();
//	const std::string& componentTableName = "components";
//
//	mysqlx::string savePoint = m_sessionManager->setSavepoint();
//
//	m_sessionManager->startTransaction();
//	RowResult res = m_sessionManager->sql(format("SHOW TABLE STATUS FROM {} LIKE '{}'", schemaName, componentTableName)).execute();
//
//	if (res.count() > 0)
//	{
//		Row row = res.fetchOne();
//		ComponentID next_auto_increment = row[10].get<int>();
//		getTable(componentTableName).insert()
//	}
//	else
//	{
//		throw std::exception(format("There is No AutoIncrement Column In {}", componentTableName).c_str());
//	}
//}