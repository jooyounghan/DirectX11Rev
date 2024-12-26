#pragma once
#include "SessionManager.h"
#include "SchemaManager.h"
#include "Scene.h"

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

public:
	ComponentManager(SessionManager* sessionManager);
	virtual ~ComponentManager();

public:
	void InitDB();

protected:
	std::unordered_map<EComponentType, std::function
		<AComponent*(const ComponentID&, const DirectX::XMFLOAT3&, const DirectX::XMFLOAT3&, const DirectX::XMFLOAT3&)>
	> m_componentTypesToMaker;
	std::unordered_map<ComponentID, AComponent*> m_componentIDToComponent;

public:
	std::function<void(const std::string&)> OnErrorOccurs = [&](const std::string&) {};

private:
	void LoadComponentMaker();
	void LoadComponents();

private:
	void LoadParentComponentsRecursive(const std::vector<ComponentID>& addedComponentIDs, mysqlx::Table& table);
	void LoadParentComponentsRecursiveImpl(mysqlx::RowResult& rowResults, AComponent* parentComponent, mysqlx::Table& table);

private:
	void AddComponent(
		const ComponentID& componentID, const EComponentType& componentType, 
		const DirectX::XMFLOAT3 position, const DirectX::XMFLOAT3 angle, const DirectX::XMFLOAT3 scale,
		AComponent* parentComponent
	);
};

