#pragma once

#include "AMovable.h"

#include "HeaderHelper.h"

#include "UploadBuffer.h"

#include "IGuiModelAcceptor.h"
#include "ISerializable.h"
#include "IUpdatable.h"
#include "IRenderable.h"

#include <string>

struct TransformationMatrix
{
	DirectX::XMMATRIX TransfomationMat;
	DirectX::XMMATRIX InvTransfomationMat;
};

class MapAsset;
class IGuiModelVisitor;


class AObject : public IOnSerializableElement, public IUpdatable, public IRenderable, public IGuiModelAcceptor, public AMovable
{
public:
	AObject();
	virtual ~AObject();

protected:
	ID3D11DeviceContext* DeviceContextCached = nullptr;

protected:
	std::string ObjectName;
	MakeGetter(ObjectName);

protected:
	std::string ObjectID;
	MakeGetter(ObjectID);

protected:
	std::shared_ptr<UploadBuffer<TransformationMatrix>> TransformationBuffer;
	MakeGetter(TransformationBuffer);

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn) override;
};

