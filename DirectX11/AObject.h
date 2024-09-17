#pragma once

#include "AMovable.h"

#include "HeaderHelper.h"
#include "IDColor.h"

#include "UploadBuffer.h"
#include "ConstantBuffer.h"

#include "ISerializable.h"
#include "IUpdatable.h"

#include <string>

class GraphicsPipeline;
class APSOObject;
class IGuiTopLevelVisitor;
class IGuiModelVisitor;

struct TransformationMatrix
{
	DirectX::XMMATRIX TransfomationMat;
	DirectX::XMMATRIX InvTransfomationMat;
};

class AObject : public IOnSerializableMap, public IUpdatable, public AMovable
{
public:
	AObject();

protected:
	std::string ObjectName;
	MakeGetter(ObjectName);

protected:
	std::string ObjectID;
	MakeGetter(ObjectID);

protected:
	ID3D11DeviceContext* DeviceContextCached = nullptr;

protected:
	bool IsRenderable = true;
	MakeSetterGetter(IsRenderable);

public:
	UploadBuffer<TransformationMatrix> TransformationBuffer;

public:
	virtual void Update(const float& DeltaTimeIn) override;
	virtual void UpdateRenderable(const bool& RenderableFlag) = 0;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;

protected:
	void SerializeString(const std::string& String, FILE* FileIn);
	void DeserializeString(std::string& String, FILE* FileIn);
};

