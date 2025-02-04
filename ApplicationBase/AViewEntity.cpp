#include "AViewEntity.h"
#include "DynamicBuffer.h"

using namespace std;
using namespace DirectX;

AViewEntity::AViewEntity()
	: m_viewProjBuffer(new DynamicBuffer(sizeof(SViewEntity), 1, &m_viewEntity))
{
}

void AViewEntity::UpdateViewEntityImpl(
	const DirectX::XMMATRIX& viewMatrix, 
	const DirectX::XMMATRIX& projectionMatrix, 
	const XMVECTOR& position
)
{
	m_viewEntity.m_viewProj = viewMatrix * projectionMatrix;
	m_viewEntity.m_invViewProj = XMMatrixInverse(nullptr, m_viewEntity.m_viewProj);
	m_viewEntity.m_viewProj = XMMatrixTranspose(m_viewEntity.m_viewProj);
	XMStoreFloat3(&m_viewEntity.m_viewPosition, position);
}
