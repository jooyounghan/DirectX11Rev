#include "ComponentManager.h"

#include "AssetManager.h"
#include "DefferedContext.h"

#include "Scene.h"
#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"

#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"

#include "ComponentDBInitializer.h"
#include "ComponentDBUpdater.h"
#include "ComponentDBRemover.h"
#include "ComponentDBCreator.h"

#include "ComponentEntityInitializer.h"
#include "ComponentEntityUpdater.h"


using namespace std;
using namespace mysqlx;
using namespace DirectX;

ComponentManager::ComponentManager(
	SessionManager* sessionManager, 
	ID3D11Device* const* deviceAddress, 
	DefferedContext* defferedContext)
	: SchemaManager(sessionManager, "component_db"),
	m_deviceAddressCached(deviceAddress),
	m_defferedContext(defferedContext)
{
	UpdateComponentToDBThread();
}

ComponentManager::~ComponentManager()
{
	m_workThreadStarted = false;
	m_workThread.join();

	unique_lock destuctLock(m_componentMutex);
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
		m_isInitialized = true;
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
				{ return new CameraComponent(componentName, componentID, position, angle, scale); }, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5
			));
			break;
		case EComponentType::SPHERE_COLLISION_COMPONENT:
			m_componentTypesToMaker.emplace(componentType, bind(
				[&](const std::string& componentName, const ComponentID& componentID, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& dummy1, const DirectX::XMFLOAT3& dummy2)
				{ return new SphereCollisionComponent(componentName, componentID, position, 0.f); }, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5
			));
			break;
		case EComponentType::ORIENTED_BOX_COLLISION_COMPONENT:
			m_componentTypesToMaker.emplace(componentType, bind(
				[&](const std::string& componentName, const ComponentID& componentID, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT3& extends)
				{ return new OrientedBoxCollisionComponent(componentName, componentID, position, angle, extends); }, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5
			));
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

	{
		unique_lock loadComponentLock(m_componentMutex);
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
	}

	{
		shared_lock parentLinkLock(m_componentMutex);
		for (auto& componentToParentID : componentsToParentID)
		{
			if (m_componentIDsToComponent.find(componentToParentID.second) != m_componentIDsToComponent.end())
			{
				AComponent* parentComponent = m_componentIDsToComponent[componentToParentID.second];
				parentComponent->AttachChildComponent(componentToParentID.first);
			}
		}
	}


	LoadLastAutoIncrementIDFromTable(componentsTableName, m_componentIssuedID);
}

void ComponentManager::LoadScenes()
{
	AssetManager* assetManager = AssetManager::GetInstance();

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

		scene->UpdateSceneMeshAsset(*assetManager);
		scene->UpdateSceneIBLMaterialAsset(*assetManager);
	}

	LoadLastAutoIncrementIDFromTable(scenesTableName, m_sceneIssuedID);
}

void ComponentManager::LoadScenesInformation()
{
	const std::string& scenesInformationsTableName = "scene_informations";
	Table scenesInformationsTable = getTable(scenesInformationsTableName, true);

	RowResult sceneInformationResults = scenesInformationsTable.select("scene_id", "component_id").lockShared().execute();

	Row sceneInformationResult;

	{
		shared_lock loadSceneLock(m_componentMutex);
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
}

void ComponentManager::InitLoadedComponents()
{
	try
	{
		ComponentDBInitializer componentDBInitializer(this);
		ComponentEntityInitializer componentEntityInitializer(*m_deviceAddressCached, m_defferedContext->GetDefferedContext());
		m_sessionManager->startTransaction();
		{
			shared_lock InitLoadedComponentsLock(m_componentMutex);
			for (auto& componentIDToComponent : m_componentIDsToComponent)
			{
				componentIDToComponent.second->Accept(&componentDBInitializer);
				componentIDToComponent.second->Accept(&componentEntityInitializer);
			}
		}

		m_sessionManager->commit();

	}
	catch (const exception& ex)
	{
		m_sessionManager->rollback();

		OnErrorOccurs(ex.what());

	}
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

void ComponentManager::UpdateComponentToDBThread()
{
	m_workThreadStarted = true;
	m_workThread = thread([&]() {
		while (m_workThreadStarted)
		{
			ComponentDBUpdater componentDBUpdater(this);
			try
			{
				m_sessionManager->startTransaction();
				{
					unique_lock writeLock(m_updateSetMutex);
					m_updateToDBComponentsWorker = m_updateToDBComponentsMain;
					m_updateToDBComponentsMain.clear();
				}

				for (AComponent* updateComponet : m_updateToDBComponentsWorker)
				{
					updateComponet->Accept(&componentDBUpdater);
				}
				m_sessionManager->commit();
				m_updateToDBComponentsWorker.clear();
			}
			catch (const std::exception& ex)
			{
				{
					unique_lock writeLock(m_updateSetMutex);
					m_updateToDBComponentsMain.insert(m_updateToDBComponentsWorker.begin(), m_updateToDBComponentsWorker.end());
					m_updateToDBComponentsWorker.clear();
				}

				m_sessionManager->rollback();

				OnErrorOccurs(ex.what());
			}

			this_thread::sleep_for(chrono::milliseconds(1000));
		}
		});
}

void ComponentManager::AddComponent(Scene* scene, AComponent* component)
{
	if (component != nullptr)
	{
		ComponentDBCreator componentDBCreator(this);

		scene->AddRootComponent(component);
		MonitorComponent(component);

		componentDBCreator.AddComponent(scene, nullptr, component);
		component->Accept(&componentDBCreator);
	}
	else
	{
		throw exception("Only Root Component Can be Inserted To Scene");
	}
}

void ComponentManager::AddComponent(AComponent* parentComponent, AComponent* component)
{
	ComponentDBCreator componentDBCreator(this);
	component->RemoveFromParent();
	parentComponent->AttachChildComponent(component);
	MonitorComponent(component);

	componentDBCreator.AddComponent(nullptr, parentComponent, component);
	component->Accept(&componentDBCreator);
}

void ComponentManager::RemoveComponent(AComponent* component)
{
	UnmonitorComponent(component);
	component->RemoveFromParent();

	const vector<AComponent*>& childComponents = component->GetChildComponents();
	for (AComponent* childComponent : childComponents)
	{
		RemoveComponent(childComponent);
	}

	ComponentDBRemover componentDBRemover(this);
	component->Accept(&componentDBRemover);
}

void ComponentManager::UpdateComponents(const float& deltaTime)
{
	if (m_isInitialized)
	{
		ComponentEntityUpdater componentEntityUpdater(m_defferedContext->GetDefferedContext(), deltaTime);

		{
			shared_lock updateComponent(m_componentMutex);
			for (auto& m_componentIDToComponent : m_componentIDsToComponent)
			{
				if (m_componentIDToComponent.second->ComsumeIsModified())
				{
					m_componentIDToComponent.second->Accept(&componentEntityUpdater);
					{
						unique_lock writeLock(m_updateSetMutex);
						m_updateToDBComponentsMain.insert(m_componentIDToComponent.second);
					}
				}
			}
		}
		m_defferedContext->RecordToCommandList();
	}
}

void ComponentManager::MonitorComponent(AComponent* component)
{
	{
		unique_lock addComponentLock(m_componentMutex);
		m_componentIDsToComponent.emplace(component->GetComponentID(), component);
	}
}

void ComponentManager::UnmonitorComponent(AComponent* component)
{
	{
		unique_lock removeComponentLock(m_componentMutex);
		m_componentIDsToComponent.erase(component->GetComponentID());
		m_updateToDBComponentsMain.erase(component);
	}
}

AComponent* ComponentManager::GetComponent(const ComponentID& componentID)
{
	if (m_componentIDsToComponent.find(componentID) != m_componentIDsToComponent.end())
	{
		return m_componentIDsToComponent.at(componentID);
	}
	return nullptr;
}
