#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "SessionManager.h"
#include "SchemaManager.h"

#include "ComponentInitializer.h"
#include "ComponentUpdater.h"
#include "ComponentRemover.h"
#include "ComponentCreator.h"

#include <queue>
#include <shared_mutex>

class Scene;
class AComponent;

enum class EComponentType
{
	NOT_SELECTED,
	STATIC_COMPONENT,
	SKELETAL_COMPONENT,
	CAMERA_COMPONENT,
};

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
		ID3D11Device** deviceAddress, 
		ID3D11DeviceContext** deviceContextAddress
	);
	virtual ~ComponentManager();

protected:
	AssetManager* m_assetManagerCached = nullptr;

protected:
	ComponentInitializer m_componentInitializer;
	ComponentUpdater m_componentUpdater;
	ComponentRemover m_componentRemover;
	ComponentCreator m_componentCreator;

protected:
	std::thread m_workThread;
	std::shared_mutex m_insertQueueMutex;
	std::shared_mutex m_removeQueueMutex;
	std::queue<AComponent*> m_insertQueue;
	std::queue<AComponent*> m_removeQueue;
	bool m_workThreadStarted = false;

protected:
	std::shared_mutex m_componentMutex;
	std::unordered_map<EComponentType, ComponentConstructor> m_componentTypesToMaker;
	std::unordered_map<ComponentID, AComponent*> m_componentIDsToComponent;

protected:
	std::unordered_map<SceneID, Scene*> m_sceneIDsToScene;
	std::unordered_map<Scene*, std::string> m_scenesToDescription;

public:
	void InitComponentManager();

private:
	void LoadComponentMakers();
	void LoadComponents();
	void LoadScenes();
	void LoadScenesInformation();
	void InitLoadedComponents();
	void LaunchComponentDBMonitor();

public:
	void AddComponent(Scene* scene, AComponent* component);
	void AddComponent(AComponent* parentComponent, AComponent* component);
	void RemoveComponent(AComponent* component);

public:
	inline const std::unordered_map<Scene*, std::string>& GetScenesWithDescription() { return m_scenesToDescription; }
};

