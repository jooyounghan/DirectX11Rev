#include "ComponentManager.h"

#include "AssetManager.h"
#include "DeferredContext.h"

#include "Scene.h"
#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"

#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"
#include "RenderControlOption.h"

#include "SpotLightComponent.h"
#include "PointLightComponent.h"
#include "LightManager.h"

#include "ComponentDBInitializer.h"
#include "ComponentDBUpdater.h"
#include "ComponentDBRemover.h"
#include "ComponentDBCreator.h"

#include "ComponentInitializer.h"
#include "ComponentUpdater.h"


using namespace std;
using namespace mysqlx;
using namespace DirectX;

ComponentManager::ComponentManager(
	SessionManager* sessionManager, 
	ID3D11Device* device,
	DeferredContext* defferedContext
)
	: SchemaManager(sessionManager, "component_db"),
	m_deviceCached(device),
	m_deferredContextCached(defferedContext)
{
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
		LoadScenes();
		LoadComponents();
		Initialize();
		UpdateComponentToDBThread();
		m_isInitialized = true;
	}
	catch (const std::exception& ex)
	{
		OnErrorOccurs(ex.what());
	}
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

void ComponentManager::LoadComponents()
{
	vector<pair<AComponent*, ComponentID>> componentsToParentID;

	LoadMeshComponent(componentsToParentID);
	LoadCameraComponent(componentsToParentID);
	LoadSphereCollisionComponentComponent(componentsToParentID);
	LoadOrientedBoxCollisionComponent(componentsToParentID);
	LoadSpotLightComponent(componentsToParentID);
	LoadPointLightComponent(componentsToParentID);

	{
		shared_lock parentLinkLock(m_componentMutex);
		for (auto& componentToParentID : componentsToParentID)
		{
			AComponent* component = componentToParentID.first;
			const SceneID& parentSceneID = component->GetParentSceneID();
			const ComponentID& componentID = component->GetComponentID();
			const ComponentID& parentComponenetID = componentToParentID.second;

			if (m_componentIDsToComponent.find(parentComponenetID) != m_componentIDsToComponent.end())
			{
				AComponent* parentComponent = m_componentIDsToComponent[parentComponenetID];
				parentComponent->AttachChildComponent(component);
			}
			else
			{
				m_sceneIDsToScene[parentSceneID]->AddRootComponent(component);
			}
		}
	}

	//LoadLastAutoIncrementIDFromTable(componentsTableName, m_componentIssuedID);
}

void ComponentManager::Initialize()
{
	shared_lock InitLoadedComponentsLock(m_componentMutex);
	ComponentDBInitializer componentDBInitializer(this);
	ComponentInitializer componentInitializer(m_deviceCached, m_deferredContextCached->GetDeferredContext());

	for (auto& m_sceneIDToScene : m_sceneIDsToScene)
	{
		m_sceneIDToScene.second->Accept(&componentInitializer);
	}

	for (auto& componentIDToComponent : m_componentIDsToComponent)
	{
		componentIDToComponent.second->Accept(&componentDBInitializer);
		componentIDToComponent.second->Accept(&componentInitializer);
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
		StartMonitoringComponent(component);

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
	StartMonitoringComponent(component);

	componentDBCreator.AddComponent(nullptr, parentComponent, component);
	component->Accept(&componentDBCreator);
}

void ComponentManager::RemoveComponent(AComponent* component)
{
	StopMonitoringComponent(component);
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
		ComponentUpdater componentUpdater(m_deferredContextCached->GetDeferredContext(), deltaTime);
		for (auto& m_sceneIDToScene : m_sceneIDsToScene)
		{
			m_sceneIDToScene.second->Accept(&componentUpdater);
		}
		
		{
			shared_lock updateComponent(m_componentMutex);
			for (auto& m_componentIDToComponent : m_componentIDsToComponent)
			{
				if (m_componentIDToComponent.second->GetModifiedOption())
				{
					m_componentIDToComponent.second->Accept(&componentUpdater);
					{
						unique_lock writeLock(m_updateSetMutex);
						m_updateToDBComponentsMain.insert(m_componentIDToComponent.second);
					}
				}
			}
		}


		m_deferredContextCached->RecordToCommandList();
	}
}

void ComponentManager::LoadMeshComponent(vector<pair<AComponent*, ComponentID>>& loadedComponentToParentIDs)
{	
	const std::string schemaName = getName();
	RowResult result = m_sessionManager->sql(
		format(
			"SELECT c.component_id, c.parent_component_id, ctt.component_type, "
			"c.component_name, c.position_x, c.position_y, c.position_z, "
			"c.angle_x, c.angle_y, c.angle_z, "
			"c.scale_x, c.scale_y, c.scale_z, c.scene_id "
			"FROM {}.components c "
			"JOIN {}.components_to_type ctt ON c.component_id = ctt.component_id "
			"WHERE ctt.component_type IN ({}, {}) LOCK IN SHARE MODE",
			schemaName, schemaName, 
			static_cast<uint32_t>(EComponentType::STATIC_COMPONENT), 
			static_cast<uint32_t>(EComponentType::SKELETAL_COMPONENT)
		)
	).execute();

	Row rowResult;
	{
		while ((rowResult = result.fetchOne()))
		{
			const ComponentID componentID = rowResult[0].get<ComponentID>();
			const ComponentID parentComponentID = rowResult[1].get<ComponentID>();
			const EComponentType componentType = static_cast<EComponentType>(rowResult[2].get<uint32_t>());
			const std::string componentName = rowResult[3].get<std::string>();

			XMFLOAT3 localPosition = XMFLOAT3(rowResult[4].get<float>(), rowResult[5].get<float>(), rowResult[6].get<float>());
			XMFLOAT3 localAngle = XMFLOAT3(rowResult[7].get<float>(), rowResult[8].get<float>(), rowResult[9].get<float>());
			XMFLOAT3 scale = XMFLOAT3(rowResult[10].get<float>(), rowResult[11].get<float>(), rowResult[12].get<float>());
			SceneID sceneID = rowResult[13].get<SceneID>();
			
			AComponent* addedComponent = nullptr;
			switch (componentType)
			{
			case EComponentType::STATIC_COMPONENT:
				addedComponent = new StaticMeshComponent(componentName, componentID, localPosition, localAngle, scale);
				break;
			case EComponentType::SKELETAL_COMPONENT:
				addedComponent = new SkeletalMeshComponent(componentName, componentID, localPosition, localAngle, scale);
				break;
			}

			AddComponentToLoadedList(loadedComponentToParentIDs, componentID, parentComponentID, sceneID, addedComponent);
		}
	}
}

void ComponentManager::LoadCameraComponent(vector<pair<AComponent*, ComponentID>>& loadedComponentToParentIDs)
{
	const std::string schemaName = getName();

	RowResult result = m_sessionManager->sql(
		format(
			"SELECT c.component_id, c.parent_component_id, ctt.component_type, c.component_name, c.position_x, c.position_y, c.position_z, "
			"c.angle_x, c.angle_y, c.angle_z, "
			"c.scale_x, c.scale_y, c.scale_z, c.scene_id, "
			"cmr.width, cmr.height, cmr.fov_angle, cmr.near_z, cmr.far_z "
			"FROM {}.components c "
			"JOIN {}.components_to_type ctt ON c.component_id = ctt.component_id "
			"JOIN {}.camera_components cmr ON c.component_id = cmr.camera_component_id "
			"LOCK IN SHARE MODE",
			schemaName, schemaName, schemaName
		)
	).execute();

	Row rowResult;
	{
		while ((rowResult = result.fetchOne()))
		{
			const ComponentID componentID = rowResult[0].get<ComponentID>();
			const ComponentID parentComponentID = rowResult[1].get<ComponentID>();
			const std::string componentName = rowResult[3].get<std::string>();

			const XMFLOAT3 localPosition = XMFLOAT3(rowResult[4].get<float>(), rowResult[5].get<float>(), rowResult[6].get<float>());
			const XMFLOAT3 localAngle = XMFLOAT3(rowResult[7].get<float>(), rowResult[8].get<float>(), rowResult[9].get<float>());
			const XMFLOAT3 scale = XMFLOAT3(rowResult[10].get<float>(), rowResult[11].get<float>(), rowResult[12].get<float>());
			const SceneID sceneID = rowResult[13].get<SceneID>();
			const uint32_t width = rowResult[14].get<uint32_t>();
			const uint32_t height = rowResult[15].get<uint32_t>();
			const float fovAngle = rowResult[16].get<float>();
			const float nearZ = rowResult[17].get<float>();
			const float farZ = rowResult[18].get<float>();


			CameraComponent* addedComponent = new CameraComponent(
				componentName, componentID, localPosition, localAngle, scale,
				width, height, fovAngle, nearZ, farZ
			);

			AddComponentToLoadedList(loadedComponentToParentIDs, componentID, parentComponentID, sceneID, addedComponent);
		}
	}
}

void ComponentManager::LoadSphereCollisionComponentComponent(vector<pair<AComponent*, ComponentID>>& loadedComponentToParentIDs)
{
	const std::string schemaName = getName();

	RowResult result = m_sessionManager->sql(
		format(
			"SELECT c.component_id, c.parent_component_id, ctt.component_type, c.component_name, c.position_x, c.position_y, c.position_z, "
			"c.angle_x, c.angle_y, c.angle_z, "
			"c.scale_x, c.scale_y, c.scale_z, c.scene_id, "
			"sphere.radius, sphere.collision_option_id "
			"FROM {}.components c "
			"JOIN {}.components_to_type ctt ON c.component_id = ctt.component_id "
			"JOIN {}.sphere_collision_components sphere ON c.component_id = sphere.sphere_collision_component_id "
			"LOCK IN SHARE MODE",
			schemaName, schemaName, schemaName
		)
	).execute();

	Row rowResult;
	{
		while ((rowResult = result.fetchOne()))
		{
			const ComponentID componentID = rowResult[0].get<ComponentID>();
			const ComponentID parentComponentID = rowResult[1].get<ComponentID>();
			const std::string componentName = rowResult[3].get<std::string>();

			const XMFLOAT3 localPosition = XMFLOAT3(rowResult[4].get<float>(), rowResult[5].get<float>(), rowResult[6].get<float>());
			const XMFLOAT3 scale = XMFLOAT3(rowResult[10].get<float>(), rowResult[11].get<float>(), rowResult[12].get<float>());
			const SceneID sceneID = rowResult[13].get<SceneID>();
			const float radius = rowResult[14].get<float>();
			const uint32_t collisionOptionID = rowResult[15].get<uint32_t>();

			SphereCollisionComponent* addedComponent = new SphereCollisionComponent(
				componentName, componentID, localPosition, scale, radius
			);
			ICollisionOption* collisionOption = CreateCollisionOption(sceneID, collisionOptionID);
			addedComponent->SetCollisionOption(collisionOption);

			AddComponentToLoadedList(loadedComponentToParentIDs, componentID, parentComponentID, sceneID, addedComponent);
		}
	}
}

void ComponentManager::LoadOrientedBoxCollisionComponent(vector<pair<AComponent*, ComponentID>>& loadedComponentToParentIDs)
{
	const std::string schemaName = getName();

	RowResult result = m_sessionManager->sql(
		format(
			"SELECT c.component_id, c.parent_component_id, ctt.component_type, c.component_name, c.position_x, c.position_y, c.position_z, "
			"c.angle_x, c.angle_y, c.angle_z, "
			"c.scale_x, c.scale_y, c.scale_z, c.scene_id, "
			"oob.extent_x, oob.extent_y, oob.extent_z, oob.collision_option_id "
			"FROM {}.components c "
			"JOIN {}.components_to_type ctt ON c.component_id = ctt.component_id "
			"JOIN {}.oriented_box_collision_components oob ON c.component_id = oob.oriented_box_collision_component_id "
			"LOCK IN SHARE MODE",
			schemaName, schemaName, schemaName
		)
	).execute();

	Row rowResult;
	{
		while ((rowResult = result.fetchOne()))
		{
			const ComponentID componentID = rowResult[0].get<ComponentID>();
			const ComponentID parentComponentID = rowResult[1].get<ComponentID>();
			const std::string componentName = rowResult[3].get<std::string>();

			const XMFLOAT3 localPosition = XMFLOAT3(rowResult[4].get<float>(), rowResult[5].get<float>(), rowResult[6].get<float>());
			const XMFLOAT3 localAngle = XMFLOAT3(rowResult[7].get<float>(), rowResult[8].get<float>(), rowResult[9].get<float>());
			const XMFLOAT3 scale = XMFLOAT3(rowResult[10].get<float>(), rowResult[11].get<float>(), rowResult[12].get<float>());
			const SceneID sceneID = rowResult[13].get<SceneID>();
			const float extentX = rowResult[14].get<float>();
			const float extentY = rowResult[15].get<float>();
			const float extentZ = rowResult[16].get<float>();
			const uint32_t collisionOptionID = rowResult[17].get<uint32_t>();

			OrientedBoxCollisionComponent* addedComponent = new OrientedBoxCollisionComponent(
				componentName, componentID, localPosition, localAngle, scale, XMFLOAT3(extentX, extentY, extentZ)
			);

			ICollisionOption* collisionOption = CreateCollisionOption(sceneID, collisionOptionID);
			addedComponent->SetCollisionOption(collisionOption);

			AddComponentToLoadedList(loadedComponentToParentIDs, componentID, parentComponentID, sceneID, addedComponent);
		}
	}
}

void ComponentManager::LoadSpotLightComponent(vector<pair<AComponent*, ComponentID>>& loadedComponentToParentIDs)
{
	const std::string schemaName = getName();

	RowResult result = m_sessionManager->sql(
		format(
			"SELECT c.component_id, c.parent_component_id, ctt.component_type, c.component_name, c.position_x, c.position_y, c.position_z, "
			"c.angle_x, c.angle_y, c.angle_z, "
			"c.scale_x, c.scale_y, c.scale_z, c.scene_id, "
			"light.light_power, light.falloff_start, light.falloff_end, light.spot_power, "
			"spot.fov_angle "
			"FROM {}.components c "
			"JOIN {}.components_to_type ctt ON c.component_id = ctt.component_id "
			"JOIN {}.light_components light ON c.component_id = light.light_component_id "
			"JOIN {}.spot_light_components spot ON c.component_id = spot.spot_light_component_id "
			"LOCK IN SHARE MODE",
			schemaName, schemaName, schemaName, schemaName
		)
	).execute();

	Row rowResult;
	{
		while ((rowResult = result.fetchOne()))
		{
			const ComponentID componentID = rowResult[0].get<ComponentID>();
			const ComponentID parentComponentID = rowResult[1].get<ComponentID>();
			const std::string componentName = rowResult[3].get<std::string>();

			const XMFLOAT3 localPosition = XMFLOAT3(rowResult[4].get<float>(), rowResult[5].get<float>(), rowResult[6].get<float>());
			const XMFLOAT3 localAngle = XMFLOAT3(rowResult[7].get<float>(), rowResult[8].get<float>(), rowResult[9].get<float>());
			const SceneID sceneID = rowResult[13].get<SceneID>();
			const float lightPower = rowResult[14].get<float>();
			const float fallOffStart = rowResult[15].get<float>();
			const float fallOffEnd = rowResult[16].get<float>();
			const float spotPower = rowResult[17].get<float>();
			const float fovAngle = rowResult[18].get<float>();

			if (m_sceneIDsToScene.find(sceneID) != m_sceneIDsToScene.end())
			{
				Scene* scene = m_sceneIDsToScene[sceneID];
				LightManager& lightManager = scene->GetLightManager();

				SpotLightComponent* addedComponent = lightManager.CreateSpotLight(
					componentName, componentID, localPosition, localAngle, 
					lightPower, fallOffStart, fallOffEnd, spotPower, 
					fovAngle
				);
				AddComponentToLoadedList(loadedComponentToParentIDs, componentID, parentComponentID, sceneID, addedComponent);
			}

		}
	}
}

void ComponentManager::LoadPointLightComponent(vector<pair<AComponent*, ComponentID>>& loadedComponentToParentIDs)
{
	const std::string schemaName = getName();

	RowResult result = m_sessionManager->sql(
		format(
			"SELECT c.component_id, c.parent_component_id, ctt.component_type, c.component_name, c.position_x, c.position_y, c.position_z, "
			"c.angle_x, c.angle_y, c.angle_z, "
			"c.scale_x, c.scale_y, c.scale_z, c.scene_id, "
			"light.light_power, light.falloff_start, light.falloff_end, light.spot_power "
			"FROM {}.components c "
			"JOIN {}.components_to_type ctt ON c.component_id = ctt.component_id "
			"JOIN {}.light_components light ON c.component_id = light.light_component_id "
			"WHERE ctt.component_type = {} "
			"LOCK IN SHARE MODE",
			schemaName, schemaName, schemaName, static_cast<uint32_t>(EComponentType::POINT_LIGHT_COMPONENT)
		)
	).execute();

	Row rowResult;
	{
		while ((rowResult = result.fetchOne()))
		{
			const ComponentID componentID = rowResult[0].get<ComponentID>();
			const ComponentID parentComponentID = rowResult[1].get<ComponentID>();
			const std::string componentName = rowResult[3].get<std::string>();

			const XMFLOAT3 localPosition = XMFLOAT3(rowResult[4].get<float>(), rowResult[5].get<float>(), rowResult[6].get<float>());
			const SceneID sceneID = rowResult[13].get<SceneID>();
			const float lightPower = rowResult[14].get<float>();
			const float fallOffStart = rowResult[15].get<float>();
			const float fallOffEnd = rowResult[16].get<float>();

			if (m_sceneIDsToScene.find(sceneID) != m_sceneIDsToScene.end())
			{
				Scene* scene = m_sceneIDsToScene[sceneID];
				LightManager& lightManager = scene->GetLightManager();

				PointLightComponent* addedComponent = lightManager.CreatePointLight(
					componentName, componentID, localPosition, 
					lightPower, fallOffStart, fallOffEnd
				);
				AddComponentToLoadedList(loadedComponentToParentIDs, componentID, parentComponentID, sceneID, addedComponent);
			}
		}
	}
}

void ComponentManager::AddComponentToLoadedList(
	vector<pair<AComponent*, ComponentID>>& loadedComponentToParentIDs, 
	const ComponentID& componentID, 
	const ComponentID& parentComponentID,
	const SceneID& sceneID,
	AComponent* component
)
{
	{
		unique_lock loadComponentLock(m_componentMutex);
		m_componentIDsToComponent.emplace(componentID, component);
	}
	component->SetParentSceneID(sceneID);

	loadedComponentToParentIDs.emplace_back(make_pair(component, parentComponentID));
}

ICollisionOption* ComponentManager::CreateCollisionOption(const uint32_t& sceneID, const uint32_t& collitionOptionID)
{
	const ECollisionOptionType collisionType = static_cast<ECollisionOptionType>(collitionOptionID);
	switch (collisionType)
	{
	case ECollisionOptionType::RENDER_CONTROL:
		return new RenderControlOption(sceneID);
		break;
	}
	return nullptr;
}


void ComponentManager::StartMonitoringComponent(AComponent* component)
{
	{
		unique_lock addComponentLock(m_componentMutex);
		m_componentIDsToComponent.emplace(component->GetComponentID(), component);
	}
}

void ComponentManager::StopMonitoringComponent(AComponent* component)
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
