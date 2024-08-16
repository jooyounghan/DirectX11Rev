#pragma once

#include "AObject.h"
#include "AttachableObject.h"
#include "StaticAssertHelper.h"

#include <list>
#include <memory>

class PlaceableObject : public AObject
{
public:
	PlaceableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn);
	virtual ~PlaceableObject();

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

public:
	virtual DirectX::XMVECTOR GetRotationQuat() const;

public:
	virtual DirectX::XMMATRIX GetScaleMatrix() const;
	virtual DirectX::XMMATRIX GetRotationMatrix() const;
	virtual DirectX::XMMATRIX GetTranslationMatrix() const;

public:
	virtual DirectX::XMMATRIX GetTransformation(const bool& IsIgnoreScale = false) const;

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;

public:
	virtual void AcceptRenderer(ARenderer* Renderer) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	template<typename Attachment, typename ...Args>
	Attachment* AddChildObject(Args... args);
};

template<typename Attachment, typename ...Args>
inline Attachment* PlaceableObject::AddChildObject(Args ...args)
{
	static_assert(std::is_base_of<AttachableObject, Attachment>::value, DerivedCondition(AttachableObject));

	AttachedChildrenObjects.emplace_back(std::make_unique<Attachment>(args...));

	Attachment* Attached = (Attachment*)AttachedChildrenObjects.back().get();
	AttachableObject* AttachedObject = (AttachableObject*)Attached;

	AttachedObject->SetParentObject(this);
	AttachedObject->SetPickingIDBufferCached(PickingIDBuffer.GetBuffer());

	return Attached;
}
