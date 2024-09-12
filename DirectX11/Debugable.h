#pragma once
#include "Vertexable.h"
#include "AIndexable.h"
#include "UploadBuffer.h"
#include "DefineType.h"

class Debugable : public AIndexable<uint16_t>, public Vertexable<DirectX::XMFLOAT3>
{
public:
	Debugable(ID3D11Device* DeviceIn);
	virtual ~Debugable();

public:
	virtual DXGI_FORMAT GetIndexFormat() override;
	virtual void Initialize() override;
};

