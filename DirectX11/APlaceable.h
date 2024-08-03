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
	SVector3D	Scale;

protected:
	std::list<std::unique_ptr<AAttachable>> ChildrenObjects;

public:
	template<typename T, typename ...Args>
	T* AddChildObjectHelper(Args... args);

protected:
	ID3D11DeviceContext* DeviceContextCached = nullptr;

public:
	virtual DirectX::XMVECTOR GetRotationQuat() const = 0;

public:
	virtual DirectX::XMMATRIX GetScaleMatrix() const = 0;
	virtual DirectX::XMMATRIX GetRotationMatrix() const = 0;
	virtual DirectX::XMMATRIX GetTranslationMatrix() const = 0;

public:
	virtual DirectX::XMMATRIX GetTransformation(const bool& IsIgnoreScale = false) const = 0;


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
	static_assert(std::is_base_of<AAttachable, T>::value, "템플릿 타입은 AAttachable의 파생 클래스여야 합니다.");
	AAttachable* AddedObject = (AAttachable*)(ChildrenObjects.back().get());
	AddedObject->SetParentObject(this);

	return (T*)AddedObject;
}
