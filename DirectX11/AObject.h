#pragma once

#include "HeaderHelper.h"
#include "DefineType.h"
#include "IDColor.h"

#include "UploadBuffer.h"
#include "ConstantBuffer.h"

#include "ISerializable.h"

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

class AObject : public IOnSerializable
{
public:
	AObject(GraphicsPipeline* GraphicsPipelineInstance);

protected:
	std::string ObjectName;
	MakeGetter(ObjectName);

protected:
	std::string ObjectID;
	MakeGetter(ObjectID);

public:
	SPosition4D	RelativePosition;
	SAngle		RelativeAngle;
	SVector3D	RelativeScale;

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
	virtual SPosition4D GetAbsolutePosition() const = 0;
	virtual SAngle GetAbsoluteAngle() const = 0;
	virtual SVector3D GetAbsoluteScale() const = 0;

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

