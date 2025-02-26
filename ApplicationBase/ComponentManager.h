#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "SchemaManager.h"
#include "ComponentType.h"

#include <queue>
#include <shared_mutex>
#include <unordered_map>

class Scene;
class AComponent;
class DeferredContext;
class ICollisionOption;
class SpotLightComponent;
class PointLightComponent;

class ComponentManager : public SchemaManager
{
	typedef uint32_t ComponentID;
	typedef uint32_t SceneID;

public:
	ComponentManager(
		SessionManager* sessionManager,
		ID3D11Device* device,
		DeferredContext* defferedContext
	);
	~ComponentManager() override;

protected:
	ID3D11Device* m_deviceCached = nullptr;
	DeferredContext* m_deferredContextCached = nullptr;

protected:
	std::thread											m_workThread;
	std::shared_mutex									m_updateSetMutex;
	std::set<AComponent*>								m_updateToDBComponentsMain;
	std::set<AComponent*>								m_updateToDBComponentsWorker;
	bool												m_workThreadStarted = false;

protected:
	bool m_isInitialized = false;
	std::unordered_map<ComponentID, AComponent*> m_componentIDsToComponent;
	std::shared_mutex							m_componentMutex;

protected:
	std::unordered_map<SceneID, Scene*> m_sceneIDsToScene;
	std::unordered_map<Scene*, std::string> m_scenesToDescription;

protected:
	uint32_t m_sceneIssuedID = NULL;
	uint32_t m_componentIssuedID = NULL;

public:
	uint32_t IssueSceneID() { return ++m_sceneIssuedID; }
	uint32_t IssueComponentID() { return ++m_componentIssuedID; }

public:
	void InitComponentManager();

private:
	void LoadScenes();
	void LoadComponents();
	void Initialize();
	void LoadLastAutoIncrementIDFromTable(const std::string& tableName, uint32_t& autoIncrementID);

private:
	void UpdateComponentToDBThread();

public:
	void AddComponent(Scene* scene, AComponent* component);
	void AddComponent(AComponent* parentComponent, AComponent* component);
	void RemoveComponent(AComponent* component);
	void UpdateComponents(const float& deltaTime);

private:
	void LoadMeshComponent(std::vector<std::pair<AComponent*, ComponentID>>& loadedComponentToParentIDs);
	void LoadCameraComponent(std::vector<std::pair<AComponent*, ComponentID>>& loadedComponentToParentIDs);
	void LoadSphereCollisionComponentComponent(std::vector<std::pair<AComponent*, ComponentID>>& loadedComponentToParentIDs);
	void LoadOrientedBoxCollisionComponent(std::vector<std::pair<AComponent*, ComponentID>>& loadedComponentToParentIDs);
	void LoadSpotLightComponent(std::vector<std::pair<AComponent*, ComponentID>>& loadedComponentToParentIDs);
	void LoadPointLightComponent(std::vector<std::pair<AComponent*, ComponentID>>& loadedComponentToParentIDs);
	void AddComponentToLoadedList(
		std::vector<std::pair<AComponent*, ComponentID>>& loadedComponentToParentIDs, 
		const ComponentID& componentID,
		const ComponentID& parentComponentID,
		const SceneID& sceneID,
		AComponent* component
	);
	ICollisionOption* CreateCollisionOption(const uint32_t& sceneID, const uint32_t& collitionOptionID);

public:
	void StartMonitoringComponent(AComponent* component);
	void StopMonitoringComponent(AComponent* component);

public:
	inline const std::unordered_map<Scene*, std::string>& GetScenesWithDescription() { return m_scenesToDescription; }
	AComponent* GetComponent(const ComponentID& componentID);
};