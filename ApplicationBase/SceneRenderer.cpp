#include "SceneRenderer.h"

SceneRenderer::SceneRenderer(PSOManager* psoManager)
	: m_psoManagerCached(psoManager)
{
}

void SceneRenderer::Visit(StaticModelComponent* staticMeshAsset)
{
}

void SceneRenderer::Visit(SkeletalModelComponent* skeletalMeshAsset)
{
}

void SceneRenderer::Visit(CameraComponent* cameraComponent)
{
}
