#include "CameraComponent.h"

using namespace DirectX;

CameraComponent::CameraComponent(
	const uint64_t& componentID, XMFLOAT3 position, 
	const float& width, const float& height, 
	const float& nearZ, const float& farZ, const float& fovAngle
)
	: ViewEntity(componentID, position, width, height, nearZ, farZ, fovAngle)
{
}

void CameraComponent::InitEntity(ID3D11Device* device)
{
	ViewEntity::InitEntity(device);
	m_film = new Texture2DInstance<SRVOption, RTVOption, UAVOption>(
		Width, Height, 1, NULL, NULL, 
		D3D11_USAGE_DEFAULT, 
		DXGI_FORMAT_R8G8B8A8_UNORM, 
		device
	);
}


