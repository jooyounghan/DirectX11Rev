#include "CameraComponent.h"

#include "Texture2DInstance.h"
#include "RTVOption.h"
#include "SRVOption.h"
#include "UAVOption.h"
#include "DSVOption.h"

#include "DynamicBuffer.h"

using namespace std;
using namespace DirectX;

CameraComponent::CameraComponent(
	const string& componentName,
	const uint32_t& componentID, 
	const XMFLOAT3& position, 
	const XMFLOAT3& angle,
	const XMFLOAT3& scale,
	const uint32_t& width, const uint32_t& height,
	const float& nearZ, const float& farZ, const float& fovAngle
)
	: AViewComponent(componentName, componentID, position, angle, scale, width, height, nearZ, farZ, fovAngle)
{
}

CameraComponent::~CameraComponent()
{
	if (m_film) delete m_film;
	if (m_idFilm) delete m_idFilm;
	if (m_idStagingFilm) delete m_idStagingFilm;
	if (m_depthStencilView) delete m_depthStencilView;
}

void CameraComponent::SetFilm(Texture2DInstance<SRVOption, RTVOption, UAVOption>* film)
{
	if (m_film) delete m_film;
	m_film = film;
}

void CameraComponent::SetIDFilm(Texture2DInstance<RTVOption>* idFilm)
{
	if (m_idFilm) delete m_idFilm;
	m_idFilm = idFilm;
}

void CameraComponent::SetIDStagingFilm(Texture2DInstance<PureTextureOption>* idStagingFilm)
{
	if (m_idStagingFilm) delete m_idStagingFilm;
	m_idStagingFilm = idStagingFilm;
}

void CameraComponent::SetDepthStencilView(Texture2DInstance<DSVOption>* depthStencilViewBuffer)
{
	if (m_depthStencilView) delete m_depthStencilView;
	m_depthStencilView = depthStencilViewBuffer;
}


void CameraComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}
