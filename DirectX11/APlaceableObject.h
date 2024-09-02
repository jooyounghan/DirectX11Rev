#pragma once

#include "AObject.h"
#include "AAttachableObject.h"
#include "IIntersectable.h"
#include "StaticAssertHelper.h"

#include <list>
#include <memory>

enum EPlaceableObjectKind : int
{
	PLACABLE_NONE,
	ACTOR_KIND,
	PAWN_KIND,
	CHARACTER_KIND,
};

class APlaceableObject : public AObject
{
	friend AAttachableObject;

public:
	APlaceableObject();
	virtual ~APlaceableObject();

protected:
	EPlaceableObjectKind PlaceableKind = EPlaceableObjectKind::PLACABLE_NONE;
	MakeSetterGetter(PlaceableKind);

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
	bool IgoreRendering = false;

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

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptRenderer(ARenderer* Renderer) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	template<typename Attachment, typename ...Args>
	Attachment* AddAttachedObject(Args... args);

public:
	void RemoveAttachedObject(AAttachableObject* AttachedObjectIn);

private:
	void RemoveFromIntersectables(AAttachableObject* AttachedObjectIn);

public:
	virtual void OnSerialize(FILE* FileIn) = 0;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;
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
	Attached->SetParentObject(this);
	Attached->SetPickingIDBufferCached(PickingIDBuffer.GetBuffer());

	return Attached;
}
