#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "SchemaManager.h"
#include "ComponentType.h"
#include "ComponentDBInitializer.h"
#include "ComponentDBUpdater.h"
#include "ComponentDBRemover.h"
#include "ComponentDBCreator.h"

#include <queue>
#include <shared_mutex>

class Scene;
class AComponent;
class DefferedContext;

class ComponentManager : public SchemaManager
{
	typedef uint32_t ComponentID;
	typedef uint32_t SceneID;
	typedef std::function<AComponent* 
		(const std::string&, const ComponentID&, const DirectX::XMFLOAT3&, const DirectX::XMFLOAT3&, const DirectX::XMFLOAT3&)
	> ComponentConstructor;

public:
	ComponentManager(
		SessionManager* sessionManager,
		AssetManager* assetManager,
		ID3D11Device* const* deviceAddress,
		DefferedContext* defferedContext
	);
	~ComponentManager() override;

protected:
	AssetManager* m_assetManagerCached = nullptr;
	ID3D11Device* const* m_deviceAddressCached = nullptr;
	DefferedContext* m_defferedContext = nullptr;

protected:
	ComponentDBInitializer m_componentDBInitializer;
	ComponentDBUpdater m_componentDBUpdater;
	ComponentDBRemover m_componentDBRemover;
	ComponentDBCreator m_componentDBCreator;

protected:
	std::thread											m_workThread;
	std::shared_mutex									m_insertToSceneQueueMutex;
	std::shared_mutex									m_insertToComponentQueueMutex;
	std::shared_mutex									m_removeQueueMutex;
	std::shared_mutex									m_updateSetMutex;
	std::queue<std::pair<Scene*, AComponent*>>			m_insertToSceneQueue;
	std::queue<std::pair<AComponent*, AComponent*>>		m_insertToComponentQueue;
	std::queue<AComponent*>								m_removeQueue;
	std::set<AComponent*>								m_updateSet;
	bool												m_workThreadStarted = false;

protected:
	bool m_isInitialized = false;
	std::unordered_map<EComponentType, ComponentConstructor> m_componentTypesToMaker;
	std::unordered_map<ComponentID, AComponent*> m_componentIDsToComponent;

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
	void LoadComponentMakers();
	void LoadComponents();
	void LoadScenes();
	void LoadScenesInformation();
	void InitLoadedComponents();
	void LoadLastAutoIncrementIDFromTable(const std::string& tableName, uint32_t& autoIncrementID);

private:
	void LaunchComponentDBMonitor();

public:
	void RegisterComponent(AComponent* component);
	void AddComponent(Scene* scene, AComponent* component);
	void AddComponent(AComponent* parentComponent, AComponent* component);
	void RemoveComponent(AComponent* component);
	void UpdateComponents();

public:
	inline const std::unordered_map<Scene*, std::string>& GetScenesWithDescription() { return m_scenesToDescription; }
	inline const std::unordered_map<ComponentID, AComponent*>& GetComponentIDsWithComponent() { return m_componentIDsToComponent; }
};

