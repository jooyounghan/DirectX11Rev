#pragma once
#include "IObject.h"
#include "UploadBuffer.h"

struct TransformationMatrix
{
	DirectX::XMMATRIX TransfomationMat;
	DirectX::XMMATRIX InvTransfomationMat;
};

class PlaceableObject : public IObject
{
public:
	PlaceableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn);
	virtual ~PlaceableObject();

protected:
	ID3D11DeviceContext* DeviceContextCached = nullptr;

public:
	UploadBuffer<TransformationMatrix> TransformationBuffer;


public:
	virtual void UpdateObject(const float& DeltaTimeIn, IObject* ParentObject = nullptr) override;
};

