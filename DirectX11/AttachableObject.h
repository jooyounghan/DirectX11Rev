#pragma once
#include "AObject.h"
#include "StaticAssertHelper.h"

#include <list>
#include <memory>

class AttachableObject : public AObject
{
public:
	AttachableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn);
	virtual~AttachableObject();

protected:
	AObject* ParentObject = nullptr;
	MakeSetterGetter(ParentObject);

protected:
	std::list<std::unique_ptr<AttachableObject>> AttachedChildrenObjects;
	MakeGetter(AttachedChildrenObjects);

protected:
	ID3D11Buffer* PickingIDBufferCached = nullptr;
	MakeSetterGetter(PickingIDBufferCached);

public:
	virtual DirectX::XMVECTOR GetRotationQuat() const override;

public:
	virtual DirectX::XMMATRIX GetScaleMatrix() const override;
	virtual DirectX::XMMATRIX GetRotationMatrix() const override;
	virtual DirectX::XMMATRIX GetTranslationMatrix() const override;

public:
	virtual DirectX::XMMATRIX GetTransformation(const bool& IsIgnoreScale = false) const override;

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;

public:
	virtual void AcceptRenderer(ARenderer* Renderer) override;;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	template<typename Attachment, typename ...Args>
	Attachment* AddChildObject(Args... args);
};

template<typename Attachment, typename ...Args>
inline Attachment* AttachableObject::AddChildObject(Args ...args)
{
	static_assert(std::is_base_of<AttachableObject, Attachment>::value, DerivedCondition(AttachableObject));

	AttachedChildrenObjects.emplace_back(std::make_unique<Attachment>(args...));

	Attachment* Attached = (Attachment*)AttachedChildrenObjects.back().get();
	AttachableObject* AttachedObject = (AttachableObject*)Attached;

	AttachedObject->SetParentObject(this);
	AttachedObject->SetPickingIDBufferCached(PickingIDBufferCached);

	return Attached;
}

