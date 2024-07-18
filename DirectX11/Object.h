#pragma once
#include "DefineType.h"
#include "HeaderHelper.h"
#include "UploadBuffer.h"

struct TransformationMatrix
{
	DirectX::XMMATRIX TransfomationMat;
	DirectX::XMMATRIX InvTransfomationMat;
};

class Object
{
public:
	Object(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn);
	~Object();

protected:
	ID3D11DeviceContext* DeviceContextCached = nullptr;

protected:
	SPosition4D	Position;
	MakeGetter(Position);

protected:
	SVector4D	Scale;
	MakeGetter(Scale);

protected:
	SAngle		Angle;
	MakeGetter(Angle);

public:
	inline void SetPosition(const float& xIn, const float& yIn, const float& zIn) { Position.x = xIn; Position.y = yIn; Position.z = zIn; Position.w = 1.f; };
	inline void SetScale(const float& xIn, const float& yIn, const float& zIn) { Scale.x = xIn; Scale.y = yIn; Scale.z = zIn; Scale.w = 1.f; };
	inline void SetAngle(const float& PitchIn, const float& RollIn, const float& YawIn) { Angle.Pitch = PitchIn; Angle.Roll = RollIn; Angle.Yaw = YawIn; }

public:
	UploadBuffer<TransformationMatrix> TransformationBuffer;

protected:
	DirectX::XMVECTOR GetRotationQuat();
	DirectX::XMMATRIX GetTransformation();

public:
	void UpdateObject();
};

