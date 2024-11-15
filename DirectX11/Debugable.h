#pragma once
#include "Vertexable.h"
#include "Indexable.h"
#include "UploadBuffer.h"

class Debugable : public Indexable<uint16_t>, public Vertexable<DirectX::XMFLOAT3>
{
public:
	Debugable(ID3D11Device* DeviceIn);
	virtual ~Debugable();

public:
	DXGI_FORMAT GetIndexFormat();
	void Initialize();
};

