#pragma once
#include "AComponent.h"

#include <type_traits>

class StaticMeshAsset;
class IBLMaterialAsset;
class IStaticMeshProvider;
class IIBLMaterialProvider;

template <typename T>
concept ComponentType = std::is_base_of_v<AComponent, T>;

class Scene
{
public:
	Scene(const uint32_t sceneID, const std::string& sceneDescription);
	~Scene() = default;

protected:
	uint32_t m_sceneID;
	std::string m_sceneDescription;
	
public:
	inline const uint32_t& GetSceneID() { return m_sceneID; }
	inline const std::string& GetSceneDescription() { return m_sceneDescription; }

protected:
	std::string m_sceneStaticMeshName;
	std::string m_iblMaterialName;

protected:
	StaticMeshAsset* m_sceneStaticMeshAsset = nullptr;
	IBLMaterialAsset* m_sceneIBLMaterialAsset = nullptr;

public:
	inline const std::string& GetSceneStaticMeshName() { return m_sceneStaticMeshName; }
	inline const std::string& GetSceneIBLMaterialName() { return m_iblMaterialName; }
	inline void SetSceneStaticMeshName(const std::string& sceneStaticMeshName) { m_sceneStaticMeshName = sceneStaticMeshName; }
	inline void SetIBLMaterialName(const std::string& iblMaterialName) { m_iblMaterialName = iblMaterialName; }
	void UpdateSceneMeshAsset(IStaticMeshProvider& staticMeshProvider);
	void UpdateSceneIBLMaterialAsset(IIBLMaterialProvider& iblMaterialProvider);

public:
	inline const StaticMeshAsset* const GetSceneMeshAsset() { return m_sceneStaticMeshAsset; }
	inline const IBLMaterialAsset* const GetIBLMaterialAsset() { return m_sceneIBLMaterialAsset; }

protected:
	std::vector<AComponent*> m_rootComponentsCached;

public:
	inline void AddRootComponent(AComponent* component) { m_rootComponentsCached.push_back(component); }

public:
	inline const std::vector<AComponent*>& GetRootComponents() { return m_rootComponentsCached; }
};
