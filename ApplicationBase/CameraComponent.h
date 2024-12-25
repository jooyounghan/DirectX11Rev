#pragma once
#include "AComponent.h"
#include "Texture2DInstance.h"
#include "RTVOption.h"
#include "SRVOption.h"
#include "UAVOption.h"

constexpr DirectX::XMVECTOR GDefaultForward = DirectX::XMVECTOR{ 0.f, 0.f, 1.f, 0.f };
constexpr DirectX::XMVECTOR GDefaultUp = DirectX::XMVECTOR{ 0.f, 1.f, 0.f, 0.f };
constexpr DirectX::XMVECTOR GDefaultRight = DirectX::XMVECTOR{ 1.f, 0.f, 0.f, 0.f };

struct SViewElement
{
	DirectX::XMMATRIX m_viewProj = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_invViewProj = DirectX::XMMatrixIdentity();
};

class CameraComponent : public AComponent, public D3D11_VIEWPORT
{
public:
	CameraComponent(
		const uint64_t& componentID, DirectX::XMFLOAT3 position,
		const uint32_t& width, const uint32_t& height,
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

protected:
	Texture2DInstance<SRVOption, RTVOption, UAVOption>* m_film = nullptr;

public:
	const Texture2DInstance<SRVOption, RTVOption, UAVOption>* const GetFilm() { return m_film; }

public:
	virtual void InitEntity(ID3D11Device* device) override;
	virtual void UpdateEntity(ID3D11Device* device, ID3D11DeviceContext* deviceContext) override;

public:
	virtual void Accept(IComponentVisitor* visitor) override;
};

