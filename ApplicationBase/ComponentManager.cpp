#include "ComponentSchema.h"

#include "StaticModelComponent.h"
#include "SkeletalModelComponent.h"
#include "CameraComponent.h"

using namespace std;
using namespace mysqlx;
using namespace DirectX;

ComponentSchema::ComponentSchema(SessionManager* sessionManager)
	: SchemaManager(sessionManager, "component_db")
{
}

ComponentSchema::~ComponentSchema()
{

}

void ComponentSchema::InitDB()
{
	LoadComponentMaker();
	LoadComponents();
}

void ComponentSchema::LoadComponentMaker()
{
	try
	{
		const std::string& componentTypeTableName = "component_types";

		Table table = getTable(componentTypeTableName, true);
		RowResult componentTypeResults = table.select("*").lockShared().execute();

		for (auto componentTypeResult : componentTypeResults)
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

void ComponentSchema::LoadComponents()
{
	try
	{
		const std::string& componentTableName = "components";
		Table table = getTable(componentTableName, true);

		RowResult componentResults = table.select("*").where("parent_component_id is NULL").lockShared().execute();
		LoadParentComponentsRecursiveImpl(componentResults, nullptr, table);

	}
	catch (const std::exception& ex)
	{
		OnErrorOccurs(ex.what());
	}
}

void ComponentSchema::LoadParentComponentsRecursive(const vector<ComponentID>& addedComponentIDs, Table& table)
{
	for (const ComponentID& addedComponentID : addedComponentIDs)
	{
		AComponent* parentComponent = nullptr;
		if (m_componentIDToComponent.find(addedComponentID) != m_componentIDToComponent.end())
		{
			parentComponent = m_componentIDToComponent[addedComponentID];
		}

		RowResult componentResults = table.select("component_id, component_type, position_x, position_y, position_z, angle_x, angle_y, angle_z, scale_x, scale_y, scale_z")
			.where("parent_component_id = :parent_component_id").bind("parent_component_id", addedComponentID).lockShared().execute();
		LoadParentComponentsRecursiveImpl(componentResults, parentComponent, table);
	}
}

void ComponentSchema::LoadParentComponentsRecursiveImpl(RowResult& rowResults, AComponent* parentComponent, Table& table)
{
	vector<ComponentID> addedComponentIDs;
	for (auto componentResult : rowResults)
	{
		const ComponentID componentID = componentResult[0].get<ComponentID>();
		const EComponentType componentType = static_cast<EComponentType>(componentResult[1].get<uint32_t>());

		XMFLOAT3 position = XMFLOAT3(componentResult[2].get<float>(), componentResult[3].get<float>(), componentResult[4].get<float>());
		XMFLOAT3 angle = XMFLOAT3(componentResult[5].get<float>(), componentResult[6].get<float>(), componentResult[7].get<float>());
		XMFLOAT3 scale = XMFLOAT3(componentResult[8].get<float>(), componentResult[9].get<float>(), componentResult[10].get<float>());

		AddComponent(componentID, componentType, position, angle, scale, parentComponent);
		addedComponentIDs.emplace_back(componentID);
	}

	LoadParentComponentsRecursive(addedComponentIDs, table);
}


void ComponentSchema::AddComponent(
	const ComponentID& componentID, const EComponentType& componentType, 
	const XMFLOAT3 position, const XMFLOAT3 angle, const XMFLOAT3 scale, 
	AComponent* parentComponent
)
{
	if (m_componentTypesToMaker.find(componentType) != m_componentTypesToMaker.end())
	{
		AComponent* addedComponent = m_componentTypesToMaker[componentType](componentID, position, angle, scale);
		m_componentIDToComponent.emplace(componentID, addedComponent);
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