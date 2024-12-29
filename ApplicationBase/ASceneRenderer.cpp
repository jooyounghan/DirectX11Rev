#include "ASceneRenderer.h"

ASceneRenderer::ASceneRenderer(
	ID3D11DeviceContext** deviceContextAddress,
	PSOManager* psoManager
)
	: m_deviceContextAddress(deviceContextAddress), m_psoManagerCached(psoManager)
{
}
