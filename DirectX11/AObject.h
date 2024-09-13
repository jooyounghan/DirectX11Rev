#pragma once

#include "HeaderHelper.h"
#include "DefineType.h"
#include "IDColor.h"

#include "UploadBuffer.h"
#include "ConstantBuffer.h"

#include "ISerializable.h"
#include "IUpdatable.h"

#include <string>

class GraphicsPipeline;
class PSOObject;
class ARenderer;
class IGuiTopLevelVisitor;
class IGuiModelVisitor;

struct TransformationMatrix
{
	DirectX::XMMATRIX TransfomationMat;
	DirectX::XMMATRIX InvTransfomationMat;
};

class AObject : public IOnSerializableMap, public IUpdatable
{
public:
	AObject();

protected:
	std::string ObjectName;
	MakeGetter(ObjectName);

protected:
	std::string ObjectID;
	MakeGetter(ObjectID);

public:
	XMFLOAT3	RelativePosition;
	XMFLOAT3	RelativeAngle;
	XMFLOAT3	RelativeScale;

protected:
	std::string RelativePositionID;
	std::string RelativeAngleID;
	std::string RelativeScaleID;
	MakeGetter(RelativePositionID);
	MakeGetter(RelativeAngleID);
	MakeGetter(RelativeScaleID);

protected:
	ID3D11DeviceContext* DeviceContextCached = nullptr;

public:
	UploadBuffer<TransformationMatrix> TransformationBuffer;

public:
	virtual DirectX::XMVECTOR GetRotationQuat() const = 0;

public:
	virtual XMFLOAT3 GetAbsolutePosition() const = 0;
	virtual XMFLOAT3 GetAbsoluteAngle() const = 0;
	virtual XMFLOAT3 GetAbsoluteScale() const = 0;

public:
	virtual DirectX::XMMATRIX GetTransformation() const = 0;

public:
	virtual void MoveForward(const float& DeltaForward) = 0;
	virtual void MoveRight(const float& DeltaRight) = 0;

public:
	virtual void RotatePitch(const float& DeltaPitch) = 0;
	virtual void RotateRoll(const float& DeltaRoll) = 0;
	virtual void RotateYaw(const float& DeltaYaw) = 0;

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptRenderer(ARenderer* Renderer) = 0;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;

protected:
	void SerializeString(const std::string& String, FILE* FileIn);
	void DeserializeString(std::string& String, FILE* FileIn);
};

