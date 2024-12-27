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
	~Scene() = default;

protected:
	std::string m_sceneStaticMeshName;
	std::string m_iblMaterialName;

protected:
	StaticMeshAsset* m_sceneMeshAsset = nullptr;
	IBLMaterialAsset* m_iblMaterialAsset = nullptr;

public:
	inline void SetSceneStaticMeshName(const std::string& sceneStaticMeshName) { m_sceneStaticMeshName = sceneStaticMeshName; }
	inline void SetIBLMaterialName(const std::string& iblMaterialName) { m_iblMaterialName = iblMaterialName; }
	void UpdateSceneMeshAsset(IStaticMeshProvider& staticMeshProvider);
	void UpdateSceneIBLMaterialAsset(IIBLMaterialProvider& iblMaterialProvider);

protected:
	std::vector<AComponent*> m_rootComponentsCached;

public:
	inline void AddRootComponent(AComponent* component) { m_rootComponentsCached.push_back(component); }

public:
	inline const std::vector<AComponent*>& GetRootComponents() { return m_rootComponentsCached; }
};
