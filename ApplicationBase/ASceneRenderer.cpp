#include "ASceneRenderer.h"
#include "ComponentPSOManager.h"

ASceneRenderer::ASceneRenderer(
	ID3D11DeviceContext* const* deviceContextAddress,
	ComponentPSOManager* componentPsoManager
)
	: m_deviceContextAddress(deviceContextAddress), m_componentPsoManagerCached(componentPsoManager)
{
}
