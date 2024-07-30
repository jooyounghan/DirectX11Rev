#pragma once
#include "IObject.h"
#include "UploadBuffer.h"

struct TransformationMatrix
{
	DirectX::XMMATRIX TransfomationMat;
	DirectX::XMMATRIX InvTransfomationMat;
};

class PlacableObject : public IObject
{
public:
	PlacableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn);
	virtual ~PlacableObject();

protected:
	ID3D11DeviceContext* DeviceContextCached = nullptr;

public:
	UploadBuffer<TransformationMatrix> TransformationBuffer;


public:
	virtual void UpdateObject(const float& DeltaTimeIn, IObject* ParentObject = nullptr) override;
};

