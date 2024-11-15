#pragma once

#include "AObject.h"
#include "AAttachableObject.h"
#include "IIntersectable.h"
#include "StaticAssertHelper.h"

#include "ConstantBuffer.h"
#include "IDColor.h"

#include <list>
#include <memory>

class APlaceableObject : public AObject
{
	friend AAttachableObject;

public:
	APlaceableObject(const std::string& PlaceableKindIn);
	virtual ~APlaceableObject();

protected:
	std::string PlaceableKind;
	MakeGetter(PlaceableKind);

protected:
	static uint32_t PickingIDIssued;

protected:
	IDColor PickingID;
	ConstantBuffer<SColor> PickingIDBuffer;
	MakeGetter(PickingID);
	MakeGetter(PickingIDBuffer);
	
protected:
	std::list<std::unique_ptr<AAttachableObject>> AttachedChildrenObjects;
	MakeGetter(AttachedChildrenObjects);

protected:
	std::list<IIntersectable*> Intersectables;
	MakeGetter(Intersectables);

public:
	virtual DirectX::XMVECTOR GetRotationQuat() const override;

public:
	virtual DirectX::XMFLOAT3 GetAbsolutePosition() const override;
	virtual DirectX::XMFLOAT3 GetAbsoluteAngle() const override;
	virtual DirectX::XMFLOAT3 GetAbsoluteScale() const override;

public:
	virtual DirectX::XMMATRIX GetTransformation() const;

public:
	virtual void MoveForward(const float& DeltaForward) override;
	virtual void MoveRight(const float& DeltaRight) override;

public:
	virtual void RotatePitch(const float& DeltaPitch) override;
	virtual void RotateRoll(const float& DeltaRoll) override;
	virtual void RotateYaw(const float& DeltaYaw) override;

protected:
	bool IsPassingTranslation = false;
	bool IsPassingRoll = false;
	bool IsPassingYaw = false;
	bool IsPassingPitch = false;
	MakePointerGetter(IsPassingTranslation);
	MakePointerGetter(IsPassingRoll);
	MakePointerGetter(IsPassingYaw);
	MakePointerGetter(IsPassingPitch);

protected:
	bool IsRenderable = true;
	MakeSetterGetter(IsRenderable);

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void Render(MapAsset* MapAssetIn) override;

public:
	template<typename Attachment, typename ...Args>
	Attachment* AddAttachedObject(Args... args);

public:
	void RemoveAttachedObject(AAttachableObject* AttachedObjectIn);

private:
	void RemoveFromIntersectables(AAttachableObject* AttachedObjectIn);
};

template<typename Attachment, typename ...Args>
inline Attachment* APlaceableObject::AddAttachedObject(Args ...args)
{
	static_assert(std::is_base_of<AAttachableObject, Attachment>::value, DerivedCondition(AAttachableObject));

	AttachedChildrenObjects.emplace_back(std::make_unique<Attachment>(args...));
	Attachment* Attached = (Attachment*)AttachedChildrenObjects.back().get();

	if (std::is_base_of<IIntersectable, Attachment>::value)
	{
		Intersectables.push_back((IIntersectable*)Attached);
	}

	AAttachableObject* AttachedObject = (AAttachableObject*)Attached;
	Attached->SetParent(this, PickingIDBuffer.GetBuffer());

	return Attached;
}
