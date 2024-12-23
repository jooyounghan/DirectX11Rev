#pragma once
#include "ComponentEntity.h"

constexpr DirectX::XMVECTOR GDefaultForward = DirectX::XMVECTOR{ 0.f, 0.f, 1.f, 0.f };
constexpr DirectX::XMVECTOR GDefaultUp = DirectX::XMVECTOR{ 0.f, 1.f, 0.f, 0.f };
constexpr DirectX::XMVECTOR GDefaultRight = DirectX::XMVECTOR{ 1.f, 0.f, 0.f, 0.f };

struct SViewElement
{
	DirectX::XMMATRIX m_viewProj = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_invViewProj = DirectX::XMMatrixIdentity();
};

class ViewEntity : public ComponentEntity, public D3D11_VIEWPORT
{
public:
	ViewEntity(
		const uint64_t& componentID, DirectX::XMFLOAT3 position, 
		const float& width, const float& height, 
		const float& nearZ, const float& farZ, const float& fovAngle
	);

protected:
	float m_nearZ;
	float m_farZ;
	float m_fovAngle;

protected:
	SViewElement m_viewElement;
	DynamicBuffer m_viewProjBuffer;

public:
	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix();

public:
	virtual void InitEntity(ID3D11Device* device);
	virtual void UpdateEntity(ID3D11DeviceContext* deviceContext);
};

