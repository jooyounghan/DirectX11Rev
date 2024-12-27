#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "SessionManager.h"
#include "SchemaManager.h"
#include "ComponentInitializer.h"

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

public:
	ComponentManager(
		SessionManager* sessionManager,
		AssetManager* assetManager,
		ID3D11Device** deviceAddress, 
		ID3D11DeviceContext** deviceContextAddress
	);
	virtual ~ComponentManager();

protected:
	ComponentInitializer m_componentInitializer;

public:
	void InitComponentManager();

protected:
	std::unordered_map<EComponentType, std::function
		<AComponent*(const std::string&, const ComponentID&, const DirectX::XMFLOAT3&, const DirectX::XMFLOAT3&, const DirectX::XMFLOAT3&)>
	> m_componentTypesToMaker;
	std::unordered_map<ComponentID, AComponent*> m_componentIDsToComponent;

protected:
	std::unordered_map<SceneID, Scene*> m_sceneIDsToScene;
	std::unordered_map<Scene*, std::string> m_scenesToDescription;

public:
	std::function<void(const std::string&)> OnErrorOccurs = [&](const std::string&) {};

private:
	void LoadComponentMakers();
	void LoadComponents();
	void LoadScenes();
	void LoadScenesInformation();

public:
	inline const std::unordered_map<Scene*, std::string>& GetScenesWithDescription() { return m_scenesToDescription; }

private:
	void LoadParentComponentsRecursive(const std::vector<ComponentID>& addedComponentIDs, mysqlx::Table& table);
	void LoadParentComponentsRecursiveImpl(mysqlx::RowResult& rowResults, AComponent* parentComponent, mysqlx::Table& table);

private:
	void AddComponent(
		const ComponentID& componentID, const EComponentType& componentType, const std::string& componentName,
		const DirectX::XMFLOAT3 position, const DirectX::XMFLOAT3 angle, const DirectX::XMFLOAT3 scale,
		AComponent* parentComponent
	);
};

