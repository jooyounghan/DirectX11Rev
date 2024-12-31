#include "ComponentManager.h"

#include "AssetManager.h"

#include "Scene.h"
#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"

using namespace std;
using namespace mysqlx;
using namespace DirectX;

ComponentManager::ComponentManager(
	SessionManager* sessionManager,
	AssetManager* assetManager,
	ID3D11Device** deviceAddress,
	ID3D11DeviceContext** deviceContextAddress
)
	: SchemaManager(sessionManager, "component_db"),
	m_assetManagerCached(assetManager),
	m_componentInitializer(assetManager, deviceAddress, this),
	m_componentUpdater(this),
	m_componentRemover(this),
	m_componentCreator(this)
{
}

ComponentManager::~ComponentManager()
{
	m_workThreadStarted = false;
	m_workThread.join();

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
	try
	{
		LoadComponentMakers();
		LoadComponents();
		LoadScenes();
		LoadScenesInformation();
		InitLoadedComponents();
		LaunchComponentDBMonitor();
	}
	catch (const std::exception& ex)
	{
		OnErrorOccurs(ex.what());
	}
}

void ComponentManager::LoadComponentMakers()
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
				[&](const std::string& componentName, const ComponentID& componentID, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT3& scale)
				{ return new StaticMeshComponent(componentName, componentID, position, angle, scale); }, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5)
			);
			break;
		case EComponentType::SKELETAL_COMPONENT:
			m_componentTypesToMaker.emplace(componentType, bind(
				[&](const std::string& componentName, const ComponentID& componentID, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT3& scale)
				{ return new SkeletalMeshComponent(componentName, componentID, position, angle, scale); }, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5)
			);
			break;
		case EComponentType::CAMERA_COMPONENT:
			m_componentTypesToMaker.emplace(componentType, bind(
				[&](const std::string& componentName, const ComponentID& componentID, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT3& scale)
				{ return new CameraComponent(componentName, componentID, position, angle, scale); }, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5)
			);
			break;
		}
	}
}

void ComponentManager::LoadComponents()
{
	const std::string& componentsTableName = "components";
	const std::string& componentsToTypeTableName = "components_to_type";
	const std::string schemaName = getName();
	RowResult result = m_sessionManager->sql(
		format(
			"SELECT c.component_id, c.parent_component_id, ctt.component_type, "
			"c.component_name, c.position_x, c.position_y, c.position_z, "
			"c.angle_x, c.angle_y, c.angle_z, "
			"c.scale_x, c.scale_y, c.scale_z "
			"FROM {}.{} c "
			"JOIN {}.{} ctt ON c.component_id = ctt.component_id LOCK IN SHARE MODE",
			schemaName, componentsTableName, schemaName, componentsToTypeTableName
		)
	).execute();

	Row rowResult;
	vector<pair<AComponent*, ComponentID>> componentsToParentID;

	while ((rowResult = result.fetchOne()))
	{
		const ComponentID componentID = rowResult[0].get<ComponentID>();
		const ComponentID parentComponentID = rowResult[1].get<ComponentID>();
		const EComponentType componentType = static_cast<EComponentType>(rowResult[2].get<uint32_t>());
		const std::string componentName = rowResult[3].get<std::string>();

		XMFLOAT3 position = XMFLOAT3(rowResult[4].get<float>(), rowResult[5].get<float>(), rowResult[6].get<float>());
		XMFLOAT3 angle = XMFLOAT3(rowResult[7].get<float>(), rowResult[8].get<float>(), rowResult[9].get<float>());
		XMFLOAT3 scale = XMFLOAT3(rowResult[10].get<float>(), rowResult[11].get<float>(), rowResult[12].get<float>());


		if (m_componentTypesToMaker.find(componentType) != m_componentTypesToMaker.end())
		{
			AComponent* addedComponent = m_componentTypesToMaker[componentType](componentName, componentID, position, angle, scale);
			m_componentIDsToComponent.emplace(componentID, addedComponent);

			componentsToParentID.emplace_back(make_pair(addedComponent, parentComponentID));
		}
	}

	for (auto& componentToParentID : componentsToParentID)
	{
		if (m_componentIDsToComponent.find(componentToParentID.second) != m_componentIDsToComponent.end())
		{
			AComponent* parentComponent = m_componentIDsToComponent[componentToParentID.second];
			parentComponent->AttachChildComponent(componentToParentID.first);
		}
	}

	LoadLastAutoIncrementIDFromTable(componentsTableName, m_componentIssuedID);
}

void ComponentManager::LoadScenes()
{
	const std::string& scenesTableName = "scenes";
	Table scenesTable = getTable(scenesTableName, true);

	RowResult sceneResults = scenesTable.select("*").lockShared().execute();

	Row sceneResult;
	while ((sceneResult = sceneResults.fetchOne()))
	{
		const SceneID sceneID = sceneResult[0].get<SceneID>();
		const std::string sceneDescription = sceneResult[1].get<std::string>();
		const std::string staticMeshName = sceneResult[2].get<std::string>();
		const std::string iblMaterialName = sceneResult[3].get<std::string>();

		Scene* scene = new Scene(sceneID, sceneDescription);
		scene->SetSceneStaticMeshName(staticMeshName);
		scene->SetIBLMaterialName(iblMaterialName);

		m_sceneIDsToScene.emplace(sceneID, scene);
		m_scenesToDescription.emplace(scene, sceneDescription);

		scene->UpdateSceneMeshAsset(*m_assetManagerCached);
		scene->UpdateSceneIBLMaterialAsset(*m_assetManagerCached);
	}

	LoadLastAutoIncrementIDFromTable(scenesTableName, m_sceneIssuedID);
}

void ComponentManager::LoadScenesInformation()
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

void ComponentManager::InitLoadedComponents()
{
	for (auto& componentIDToComponent : m_componentIDsToComponent)
	{
		componentIDToComponent.second->Accept(&m_componentInitializer);
	}
}

void ComponentManager::LaunchComponentDBMonitor()
{
	m_workThreadStarted = true;
	m_workThread = thread([&]() {
		while (m_workThreadStarted)
		{
			std::queue<pair<Scene*, AComponent*>>		tempInsertToSceneQueue;
			std::queue<pair<AComponent*, AComponent*>>	tempInsertToComponentQueue;
			std::queue<AComponent*>						tempRemoveQueue;
;
			try
			{
				m_sessionManager->startTransaction();
				{
					{
						unique_lock writeLock(m_insertToSceneQueueMutex);

						while (!m_insertToSceneQueue.empty())
						{
							tempInsertToSceneQueue.push(m_insertToSceneQueue.front());
							m_insertToSceneQueue.pop();
							
							auto& sceneWithComponent = tempInsertToSceneQueue.back();
							Scene* scene = sceneWithComponent.first;
							AComponent* component = sceneWithComponent.second;

							m_componentCreator.AddComponent(scene, nullptr, component);
							component->Accept(&m_componentCreator);
						}
					}
					
					{
						unique_lock writeLock(m_insertToComponentQueueMutex);

						while (!m_insertToComponentQueue.empty())
						{
							tempInsertToComponentQueue.push(m_insertToComponentQueue.front());
							m_insertToComponentQueue.pop();

							auto& parentComponentWithComponent = tempInsertToComponentQueue.back();
							AComponent* parentComponent = parentComponentWithComponent.first;
							AComponent* component = parentComponentWithComponent.second;

							m_componentCreator.AddComponent(nullptr, parentComponent, component);
							component->Accept(&m_componentCreator);
						}
					}

					{
						unique_lock writeLock(m_removeQueueMutex);

						while (!m_removeQueue.empty())
						{
							tempRemoveQueue.push(m_removeQueue.front());
							m_removeQueue.pop();
							tempRemoveQueue.back()->Accept(&m_componentRemover);
						}
					}
				}

				{
					shared_lock readLock(m_componentMutex);
					for (auto& m_componentIDToComponent : m_componentIDsToComponent)
					{
						if (m_componentIDToComponent.second->ComsumeIsModified())
						{
							m_componentIDToComponent.second->Accept(&m_componentUpdater);
						}
					}
				}

				m_sessionManager->commit();

				while (!tempRemoveQueue.empty())
				{
					delete tempRemoveQueue.front();
					tempRemoveQueue.pop();
				}

				this_thread::sleep_for(chrono::seconds(1));
			}
			catch (const std::exception& ex)
			{
				{
					unique_lock writeLock(m_insertToSceneQueueMutex);
					while (!tempInsertToSceneQueue.empty())
					{
						m_insertToSceneQueue.push(tempInsertToSceneQueue.front());
						tempInsertToSceneQueue.pop();
					}
				}

				{
					unique_lock writeLock(m_insertToComponentQueueMutex);
					while (!tempInsertToComponentQueue.empty())
					{
						m_insertToComponentQueue.push(tempInsertToComponentQueue.front());
						tempInsertToComponentQueue.pop();
					}
				}

				{
					unique_lock writeLock(m_removeQueueMutex);
					while (!tempRemoveQueue.empty())
					{
						m_removeQueue.push(tempRemoveQueue.front());
						tempRemoveQueue.pop();
					}
				}

				m_sessionManager->rollback();

				OnErrorOccurs(ex.what());
			}
		}
	});

}

void ComponentManager::LoadLastAutoIncrementIDFromTable(const std::string& tableName, uint32_t& autoIncrementID)
{
	const std::string schemaName = getName();

	RowResult res = m_sessionManager->sql(
		format("SHOW TABLE STATUS FROM {} LIKE '{}'", schemaName, tableName)
	).execute();

	if (res.count() > 0)
	{
		Row row = res.fetchOne();
		uint32_t next_auto_increment = row[10].get<int>();
		autoIncrementID = next_auto_increment;
	}
	else
	{
		throw std::exception(format("There is No AutoIncrement Column In {}", tableName).c_str());
	}
}


void ComponentManager::AddComponent(Scene* scene, AComponent* component)
{
	if (component != nullptr)
	{
		scene->AddRootComponent(component);
		m_componentIDsToComponent.emplace(component->GetComponentID(), component);

		{
			unique_lock writeLock(m_insertToSceneQueueMutex);
			m_insertToSceneQueue.push(make_pair(scene, component));
		}
	}
	else
	{
		throw exception("Only Root Component Can be Inserted To Scene");
	}
}

void ComponentManager::AddComponent(AComponent* parentComponent, AComponent* component)
{
	component->RemoveFromParent();
	parentComponent->AttachChildComponent(component);
	m_componentIDsToComponent.emplace(component->GetComponentID(), component);

	{
		unique_lock writeLock(m_insertToComponentQueueMutex);
		m_insertToComponentQueue.push(make_pair(parentComponent, component));
	}
}

void ComponentManager::RemoveComponent(AComponent* component)
{
	m_componentIDsToComponent.erase(component->GetComponentID());
	component->RemoveFromParent();


	const vector<AComponent*>& childComponents = component->GetChildComponents();
	for (AComponent* childComponent : childComponents)
	{
		RemoveComponent(childComponent);
	}

	{
		unique_lock writeLock(m_removeQueueMutex);
		m_removeQueue.push(component);
	}
}
