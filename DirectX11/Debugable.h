#pragma once
#include "Vertexable.h"
#include "IIndexable.h"
#include "UploadBuffer.h"
#include "DefineType.h"

enum class EDebugObjectType
{
	Sphere,
	Box,
	Capsule,
	NumDebugObject
};

constexpr size_t GetDebugObjectTypeAsIndex(EDebugObjectType DebugObjectType) { return static_cast<size_t>(DebugObjectType); }
constexpr EDebugObjectType GetIndexAsDebugObjectType(size_t DebugIndex) { return static_cast<EDebugObjectType>(DebugIndex); }

struct DebugVertex
{
	DirectX::XMFLOAT3 RelativePosition;
};

class Debugable : public IIndexable<uint16_t>, public Vertexable<DebugVertex>
{
public:
	Debugable(ID3D11Device* DeviceIn);
	virtual ~Debugable();

public:
	virtual DXGI_FORMAT GetIndexFormat() override;
	virtual void Initialize(ID3D11Device* DeviceIn) override;
};

