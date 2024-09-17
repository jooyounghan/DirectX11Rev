#pragma once
#include "HeaderHelper.h"
#include <DirectXMath.h>
#include <string>

class AMovable
{
public:
	AMovable();

public:
	DirectX::XMFLOAT3	RelativePosition;
	DirectX::XMFLOAT3	RelativeAngle;
	DirectX::XMFLOAT3	RelativeScale;

protected:
	std::string RelativePositionID;
	std::string RelativeAngleID;
	std::string RelativeScaleID;
	MakeGetter(RelativePositionID);
	MakeGetter(RelativeAngleID);
	MakeGetter(RelativeScaleID);

public:
	virtual DirectX::XMVECTOR GetRotationQuat() const = 0;

public:
	virtual DirectX::XMFLOAT3 GetAbsolutePosition() const = 0;
	virtual DirectX::XMFLOAT3 GetAbsoluteAngle() const = 0;
	virtual DirectX::XMFLOAT3 GetAbsoluteScale() const = 0;

public:
	virtual DirectX::XMMATRIX GetTransformation() const = 0;

public:
	virtual void MoveForward(const float& DeltaForward) = 0;
	virtual void MoveRight(const float& DeltaRight) = 0;

public:
	virtual void RotatePitch(const float& DeltaPitch) = 0;
	virtual void RotateRoll(const float& DeltaRoll) = 0;
	virtual void RotateYaw(const float& DeltaYaw) = 0;
};