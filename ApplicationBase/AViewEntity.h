#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class DynamicBuffer;

constexpr DirectX::XMVECTOR GDefaultForward = DirectX::XMVECTOR{ 0.f, 0.f, 1.f, 0.f };
constexpr DirectX::XMVECTOR GDefaultUp = DirectX::XMVECTOR{ 0.f, 1.f, 0.f, 0.f };
constexpr DirectX::XMVECTOR GDefaultRight = DirectX::XMVECTOR{ 1.f, 0.f, 0.f, 0.f };

struct SViewEntity
{
	DirectX::XMMATRIX m_viewProj = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_invViewProj = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT3 m_viewPosition = { 0.f, 0.f, 0.f };
	float m_dummy = 0.f;
};

class AViewEntity
{
public:
	AViewEntity();
	virtual ~AViewEntity() = default;

protected:
	SViewEntity m_viewEntity;
	DynamicBuffer* m_viewProjBuffer = nullptr;

public:
	inline DynamicBuffer* GetViewProjMatrixBuffer() const { return m_viewProjBuffer; };

public:
	virtual DirectX::XMMATRIX GetViewMatrix() const = 0;
	virtual DirectX::XMMATRIX GetProjectionMatrix() const = 0;
	virtual void UpdateViewEntity() = 0;

protected:
	void UpdateViewEntityImpl(
		const DirectX::XMMATRIX& viewMatrix, 
		const DirectX::XMMATRIX& projectionMatrix, 
		const DirectX::XMVECTOR& position
	);
};