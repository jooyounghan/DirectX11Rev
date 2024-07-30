#pragma once
#include "DefineType.h"
#include "HeaderHelper.h"
#include <list>
#include <memory>

class IObject
{
public:
	IObject();

public:
	SPosition4D	Position;
	SAngle		Angle;
	float		Scale;

protected:
	bool IsRelativeToParent = false;

protected:
	std::list<std::unique_ptr<IObject>> ChildrenObject;

public:
	DirectX::XMVECTOR GetRotationQuat() const;
	
public:
	DirectX::XMMATRIX GetScaleMatrix() { return DirectX::XMMatrixScaling(Scale, Scale, Scale); }
	DirectX::XMMATRIX GetRotartionMatrix() { return DirectX::XMMatrixRotationRollPitchYaw(Angle.Pitch, Angle.Yaw, Angle.Roll); }
	DirectX::XMMATRIX GetTranslationMatrix() { return DirectX::XMMatrixTranslation(Position.x, Position.y, Position.z); }

public:
	DirectX::XMMATRIX GetTransformation() const;

public:
	virtual void UpdateObject(const float& DeltaTimeIn, IObject* ParentObject = nullptr) = 0;

public:
	template<typename T, typename ...Args>
	T* ChildObjectAddHelper(Args... args)
	{
		ChildrenObject.emplace_back(std::make_unique<T>(args...));
		static_assert(std::is_base_of<IObject, T>::value, "템플릿 타입은 Object의 파생 클래스여야 합니다.");
		T* AddedObject = (T*)(ChildrenObject.back().get());
		return AddedObject;
	}
};

