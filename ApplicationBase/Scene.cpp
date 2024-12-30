#include "Scene.h"

Scene::Scene(const uint32_t sceneID, const std::string& sceneDescription)
	: m_sceneID(sceneID), m_sceneDescription(sceneDescription)
{
}

void Scene::UpdateSceneMeshAsset(IStaticMeshProvider& staticMeshProvider)
{
	staticMeshProvider.GetStaticMeshAsset(m_sceneStaticMeshName);
}

void Scene::UpdateSceneIBLMaterialAsset(IIBLMaterialProvider& iblMaterialProvider)
{
	iblMaterialProvider.GetIBLMaterialAsset(m_iblMaterialName);

}
