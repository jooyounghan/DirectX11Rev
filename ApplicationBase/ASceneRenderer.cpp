#include "ASceneRenderer.h"

ASceneRenderer::ASceneRenderer(
	ID3D11DeviceContext** deviceContextAddress,
	ComponentPSOManager* componentPsoManager
)
	: m_deviceContextAddress(deviceContextAddress), m_componentPsoManagerCached(componentPsoManager)
{
}
