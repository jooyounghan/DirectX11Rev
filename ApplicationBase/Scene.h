#pragma once
#include "IBLMaterialAsset.h"
#include "StaticMeshAsset.h"
#include "AComponent.h"

#include <type_traits>

template <typename T>
concept ComponentType = std::is_base_of_v<AComponent, T>;

class Scene
{
public:
	Scene() = default;
	~Scene();

protected:
	StaticMeshAsset* m_sceneMeshAsset = nullptr;
	IBLMaterialAsset* m_iblMaterialAsset = nullptr;

protected:
	std::vector<AComponent*> m_rootComponents;

public:
	inline const std::vector<AComponent*>& GetRootComponents() { return m_rootComponents; }

public:
	template<typename ComponentType, typename ...Args>
	void AddRootComponent(Args&&...);
};

template<typename ComponentType, typename ...Args>
inline void Scene::AddRootComponent(Args&&... args)
{
	m_rootComponents.emplace_back(new ComponentType(std::forward(args)));
}
