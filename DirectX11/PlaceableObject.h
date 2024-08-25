#pragma once

#include "AObject.h"
#include "AttachableObject.h"
#include "IIntersectable.h"
#include "StaticAssertHelper.h"

#include <list>
#include <memory>

enum EPlaceableObjectKind : int
{
	PlaceableNone,
	ActorKind,
	PawnKind,
	CharacterKind,
};

class PlaceableObject : public AObject
{
	friend AttachableObject;

public:
	PlaceableObject(GraphicsPipeline* GraphicsPipelineInstance);
	virtual ~PlaceableObject();

protected:
	EPlaceableObjectKind PlaceableKind = EPlaceableObjectKind::PlaceableNone;
	MakeSetterGetter(PlaceableKind);

protected:
	static uint32_t PickingIDIssued;

protected:
	IDColor PickingID;
	ConstantBuffer<SColor> PickingIDBuffer;
	MakeGetter(PickingID);
	MakeGetter(PickingIDBuffer);
	
protected:
	std::list<std::unique_ptr<AttachableObject>> AttachedChildrenObjects;
	MakeGetter(AttachedChildrenObjects);

protected:
	std::list<IIntersectable*> Intersectables;
	MakeGetter(Intersectables);

public:
	bool IgoreRendering = false;

public:
	virtual DirectX::XMVECTOR GetRotationQuat() const;

public:
	virtual SPosition4D GetAbsolutePosition() const override;
	virtual SAngle GetAbsoluteAngle() const override;
	virtual SVector3D GetAbsoluteScale() const override;

public:
	virtual DirectX::XMMATRIX GetTransformation() const;

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;

public:
	virtual void AcceptRenderer(ARenderer* Renderer) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	template<typename Attachment, typename ...Args>
	Attachment* AddAttachedObject(Args... args);

public:
	void RemoveAttachedObject(AttachableObject* AttachedObjectIn);

public:
	virtual void OnSerialize(FILE* FileIn) = 0;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;
};

template<typename Attachment, typename ...Args>
inline Attachment* PlaceableObject::AddAttachedObject(Args ...args)
{
	static_assert(std::is_base_of<AttachableObject, Attachment>::value, DerivedCondition(AttachableObject));

	AttachedChildrenObjects.emplace_back(std::make_unique<Attachment>(args...));
	Attachment* Attached = (Attachment*)AttachedChildrenObjects.back().get();

	if (std::is_base_of<IIntersectable, Attachment>::value)
	{
		Intersectables.push_back((IIntersectable*)Attached);
	}

	AttachableObject* AttachedObject = (AttachableObject*)Attached;
	Attached->SetParentObject(this);
	Attached->SetPickingIDBufferCached(PickingIDBuffer.GetBuffer());

	return Attached;
}
