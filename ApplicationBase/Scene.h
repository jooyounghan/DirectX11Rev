#pragma once
#include "ISceneVisitor.h"

#include <string>
#include <vector>
#include <type_traits>

class AComponent;
class StaticMeshAsset;
class IBLMaterialAsset;
class IStaticMeshProvider;
class IIBLMaterialProvider;
class SpotLightComponent;
class PointLightComponent;

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
	std::vector<SpotLightComponent*> m_spotLights;
	std::vector<PointLightComponent*> m_pointLights;

public:
	inline void AddRootComponent(AComponent* component) { m_rootComponentsCached.push_back(component); }
	inline void AddSpotLight(SpotLightComponent* spotLight) { m_spotLights.push_back(spotLight); }
	inline void AddPointLight(PointLightComponent* pointLight) { m_pointLights.push_back(pointLight); }

public:
	inline const std::vector<AComponent*>& GetRootComponents() { return m_rootComponentsCached; }
	inline const std::vector<SpotLightComponent*>& GetSpotLights() { return m_spotLights; }
	inline const std::vector<PointLightComponent*>& GetPointLights() { return m_pointLights; }

public:
	void Accept(ISceneVisitor* visitor);
};
