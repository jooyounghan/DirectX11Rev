#include "SceneRenderer.h"

SceneRenderer::SceneRenderer(PSOManager* psoManager)
	: m_psoManagerCached(psoManager)
{
}

void SceneRenderer::Visit(StaticMeshComponent* staticMeshAsset)
{
}

void SceneRenderer::Visit(SkeletalMeshComponent* skeletalMeshAsset)
{
}

void SceneRenderer::Visit(CameraComponent* cameraComponent)
{
}
