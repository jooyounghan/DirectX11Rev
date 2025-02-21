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
class SpotLightComponent;
class PointLightComponent;

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
		ID3D11Device* const* deviceAddress,
		DeferredContext* DeferredContext
	);
	~ComponentManager() override;

protected:
	ID3D11Device* const* m_deviceAddressCached = nullptr;
	DeferredContext* m_DeferredContext = nullptr;

protected:
	std::thread											m_workThread;
	std::shared_mutex									m_updateSetMutex;
	std::set<AComponent*>								m_updateToDBComponentsMain;
	std::set<AComponent*>								m_updateToDBComponentsWorker;
	bool												m_workThreadStarted = false;

protected:
	bool m_isInitialized = false;
	std::unordered_map<EComponentType, ComponentConstructor> m_componentTypesToMaker;
	std::unordered_map<ComponentID, AComponent*> m_componentIDsToComponent;
	std::unordered_map<ComponentID, SpotLightComponent*> m_componentIDsToSpotLight;
	std::unordered_map<ComponentID, PointLightComponent*> m_componentIDsToPointLight;
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
	AComponent* CreateComponent(
		const ComponentID& componentID, const ComponentID& parentComponentID,
		const EComponentType& componentType, const std::string& componentName,
		const DirectX::XMFLOAT3& localPosition,
		const DirectX::XMFLOAT3& localAngle,
		const DirectX::XMFLOAT3& localScale,
		const SceneID& sceneID
	);
	template<typename T, typename ...Args>
	T* CreateComponentImpl(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const DirectX::XMFLOAT3& localAngle,
		const DirectX::XMFLOAT3& scale,
		Args&& ...args
	);

public:
	void MonitorComponent(AComponent* component);
	void UnmonitorComponent(AComponent* component);

public:
	inline const std::unordered_map<Scene*, std::string>& GetScenesWithDescription() { return m_scenesToDescription; }
	AComponent* GetComponent(const ComponentID& componentID);
};