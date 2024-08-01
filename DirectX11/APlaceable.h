#pragma once
#include "IObject.h"
#include "DefineType.h"
#include "UploadBuffer.h"
#include "Attachable.h"

#include <list>
#include <memory>

struct TransformationMatrix
{
	DirectX::XMMATRIX TransfomationMat;
	DirectX::XMMATRIX InvTransfomationMat;
};

class APlaceable : virtual public IObject
{
public:
	APlaceable(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn);
	virtual ~APlaceable();

public:
	SPosition4D	Position;
	SAngle		Angle;
	float		Scale;

protected:
	std::list<std::unique_ptr<AAttachable>> ChildrenObjects;

public:
	template<typename T, typename ...Args>
	T* AddChildObjectHelper(Args... args);

protected:
	ID3D11DeviceContext* DeviceContextCached = nullptr;

public:
	DirectX::XMVECTOR GetRotationQuat() const;

public:
	DirectX::XMMATRIX GetScaleMatrix() { return DirectX::XMMatrixScaling(Scale, Scale, Scale); }
	DirectX::XMMATRIX GetRotartionMatrix() { return DirectX::XMMatrixRotationRollPitchYaw(Angle.Pitch, Angle.Yaw, Angle.Roll); }
	DirectX::XMMATRIX GetTranslationMatrix() { return DirectX::XMMatrixTranslation(Position.x, Position.y, Position.z); }

public:
	DirectX::XMMATRIX GetTransformation() const;


public:
	UploadBuffer<TransformationMatrix> TransformationBuffer;

public:
	virtual void Render(PSOObject* PSOObjectIn) = 0;
	virtual void UpdateObject(const float& DeltaTimeIn) = 0;
};






template<typename T, typename ...Args>
inline T* APlaceable::AddChildObjectHelper(Args ...args)
{
	ChildrenObjects.emplace_back(std::make_unique<T>(args...));
	static_assert(std::is_base_of<AAttachable, T>::value, "���ø� Ÿ���� AAttachable�� �Ļ� Ŭ�������� �մϴ�.");
	AAttachable* AddedObject = (AAttachable*)(ChildrenObjects.back().get());
	AddedObject->SetParentObject(this);

	return (T*)AddedObject;
}
