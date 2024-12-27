#include "Scene.h"

void Scene::UpdateSceneMeshAsset(IStaticMeshProvider& staticMeshProvider)
{
	staticMeshProvider.GetStaticMeshAsset(m_sceneStaticMeshName);
}

void Scene::UpdateSceneIBLMaterialAsset(IIBLMaterialProvider& iblMaterialProvider)
{
	iblMaterialProvider.GetIBLMaterialAsset(m_iblMaterialName);
}
