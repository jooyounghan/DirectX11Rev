#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "DynamicBuffer.h"

struct SComponentElement
{
	SComponentElement(const uint64_t& componentID) : m_componentID(componentID) { }
	const uint64_t m_componentID;
	DirectX::XMMATRIX m_transformation = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_invTransformation = DirectX::XMMatrixIdentity();
};

class ComponentEntity
{
public:
	ComponentEntity(const uint64_t& componentID, DirectX::XMFLOAT3 position);

protected:
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_angle;
	DirectX::XMVECTOR m_scale;
	
protected:
	SComponentElement m_componentElement;
	DynamicBuffer m_transformationBuffer;

public:
	DirectX::XMMATRIX GetTranformation();
	DirectX::XMVECTOR GetQuaternion();

public:
	virtual void InitEntity(ID3D11Device* device);
	virtual void UpdateEntity(ID3D11DeviceContext* deviceContext);
};

