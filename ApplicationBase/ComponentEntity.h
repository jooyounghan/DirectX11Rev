#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "DynamicBuffer.h"
#include "ConstantBuffer.h"

struct STransformation
{
	DirectX::XMMATRIX m_transformation = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_invTransformation = DirectX::XMMatrixIdentity();
};

struct SComponent
{
	SComponent(const uint32_t& componentID);;
	const uint32_t m_componentID;
	const uint32_t m_dummy1;
	const uint32_t m_dummy2;
	const uint32_t m_dummy3;
};

class ComponentEntity
{
public:
	ComponentEntity(
		const uint32_t& componentID, 
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale
	);

protected:
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_angle;
	DirectX::XMVECTOR m_scale;
	
public:
	float* GetPositionAddress() { return m_position.m128_f32; }
	float* GetAngleAddress() { return m_angle.m128_f32; }
	float* GetScaleAddress() { return m_scale.m128_f32; }

protected:
	STransformation m_transformation;
	DynamicBuffer m_transformationBuffer;

protected:
	SComponent m_componentConstant;
	ConstantBuffer m_componentBuffer;

public:
	inline const uint32_t& GetComponentID() { return m_componentConstant.m_componentID; }

public:
	DirectX::XMMATRIX GetTranformation();
	DirectX::XMVECTOR GetQuaternion();

public:
	virtual void InitEntity(ID3D11Device* device);
	virtual void UpdateEntity(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
};

