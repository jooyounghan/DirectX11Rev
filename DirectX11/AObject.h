#pragma once

#include "AMovable.h"

#include "HeaderHelper.h"
#include "IDColor.h"

#include "UploadBuffer.h"
#include "ConstantBuffer.h"

#include "ISerializable.h"
#include "IUpdatable.h"

#include <string>

class MapAsset;
class IGuiModelVisitor;

struct TransformationMatrix
{
	DirectX::XMMATRIX TransfomationMat;
	DirectX::XMMATRIX InvTransfomationMat;
};

class AObject : public IOnSerializableMap, public IUpdatable, public AMovable
{
public:
	AObject(MapAsset* MapAssetInstance);

protected:
	ID3D11DeviceContext* DeviceContextCached = nullptr;

protected:
	MapAsset* MapAssetCached = nullptr;

protected:
	std::string ObjectName;
	MakeGetter(ObjectName);

protected:
	std::string ObjectID;
	MakeGetter(ObjectID);

public:
	UploadBuffer<TransformationMatrix> TransformationBuffer;

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void Render() = 0;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;

public:
	static void SerializeString(const std::string& String, FILE* FileIn);
	static void DeserializeString(std::string& String, FILE* FileIn);
};

