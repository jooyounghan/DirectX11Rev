#pragma once

#include "HeaderHelper.h"
#include "DefineType.h"
#include "IDColor.h"

#include "UploadBuffer.h"
#include "ConstantBuffer.h"

#include "ISerializable.h"

#include <string>

class PSOObject;
class ARenderer;
class IGuiTopLevelVisitor;
class IGuiModelVisitor;

struct TransformationMatrix
{
	DirectX::XMMATRIX TransfomationMat;
	DirectX::XMMATRIX InvTransfomationMat;
};

class AObject : public IOnSerializable
{
public:
	AObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn);

protected:
	std::string ObjectName;
	MakeGetter(ObjectName);

protected:
	std::string ObjectID;
	MakeGetter(ObjectID);

public:
	SPosition4D	Position;
	SAngle		Angle;
	SVector3D	Scale;

protected:
	ID3D11DeviceContext* DeviceContextCached = nullptr;

public:
	UploadBuffer<TransformationMatrix> TransformationBuffer;

public:
	virtual DirectX::XMVECTOR GetRotationQuat() const = 0;

public:
	virtual DirectX::XMMATRIX GetScaleMatrix() const = 0;
	virtual DirectX::XMMATRIX GetRotationMatrix() const = 0;
	virtual DirectX::XMMATRIX GetTranslationMatrix() const = 0;

public:
	virtual DirectX::XMMATRIX GetTransformation() const = 0;


public:
	virtual void UpdateObject(const float& DeltaTimeIn);

public:
	virtual void AcceptRenderer(ARenderer* Renderer) = 0;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

