#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "DirectionEntity.h"

constexpr float GDefaultNearZ = 0.1f;
constexpr float GDefaultFarZ = 1E6f;
constexpr float GDefaultFovAngle = 60.f;
constexpr uint32_t GDefaultViewWidth = 1280;
constexpr uint32_t GDefaultViewHeight = 960;

struct SViewEntity
{
	DirectX::XMMATRIX m_viewProj = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_invViewProj = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT3 m_viewPosition = { 0.f, 0.f, 0.f };
	float m_dummy = 0.f;
};