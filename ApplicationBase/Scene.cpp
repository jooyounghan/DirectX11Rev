#include "Scene.h"
#include "IBLMaterialAsset.h"
#include "StaticMeshAsset.h"

Scene::Scene(const uint32_t sceneID, const std::string& sceneDescription)
	: m_sceneID(sceneID), m_sceneDescription(sceneDescription)
{
}

void Scene::UpdateSceneMeshAsset(IStaticMeshProvider& staticMeshProvider)
{
	m_sceneStaticMeshAsset = staticMeshProvider.GetStaticMeshAsset(m_sceneStaticMeshName);
}

void Scene::UpdateSceneIBLMaterialAsset(IIBLMaterialProvider& iblMaterialProvider)
{
	m_sceneIBLMaterialAsset = iblMaterialProvider.GetIBLMaterialAsset(m_iblMaterialName);
}
