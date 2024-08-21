#pragma once
#include "AObject.h"
#include "StaticAssertHelper.h"
#include "IIntersectable.h"

#include <list>
#include <memory>

class PlaceableObject;

enum EAttachableObjectKind : int
{
	AttachableNone,
	MeshObjectKind,
	BoundingSphereKind,
	OrientedBoundingBoxKind,
};

class AttachableObject : public AObject
{

public:
	AttachableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn);
	virtual~AttachableObject();

protected:
	EAttachableObjectKind AttachableKind = EAttachableObjectKind::AttachableNone;
	MakeSetterGetter(AttachableKind);

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
	virtual DirectX::XMMATRIX GetTransformation() const override;

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;

public:
	virtual void AcceptRenderer(ARenderer* Renderer) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	template<typename Attachment, typename ...Args>
	Attachment* AddAttachedObject(Args... args);

private:
	void AddIntersectableToRootPlaceable(PlaceableObject* RootPlaceable, IIntersectable* Intersectable);

public:
	void RemoveAttachedObject(AttachableObject* AttachedObjectIn);


public:
	virtual void OnSerialize(FILE* FileIn) = 0;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;
};

template<typename Attachment, typename ...Args>
inline Attachment* AttachableObject::AddAttachedObject(Args ...args)
{
	static_assert(std::is_base_of<AttachableObject, Attachment>::value, DerivedCondition(AttachableObject));

	AttachedChildrenObjects.emplace_back(std::make_unique<Attachment>(args...));
	Attachment* Attached = (Attachment*)AttachedChildrenObjects.back().get();

	if (std::is_base_of<IIntersectable, Attachment>::value)
	{
		AObject* CurrentParent = ParentObject;
		AttachableObject* AttachableParent = nullptr;
		PlaceableObject* PlaceableParent = nullptr;

		AttachableParent = dynamic_cast<AttachableObject*>(ParentObject);
		while (AttachableParent != nullptr)
		{
			CurrentParent = AttachableParent->ParentObject;
			AttachableParent = dynamic_cast<AttachableObject*>(AttachableParent->ParentObject);
		}

		PlaceableParent = (PlaceableObject*)CurrentParent;
		AddIntersectableToRootPlaceable(PlaceableParent, (IIntersectable*)Attached);
	}

	AttachableObject* AttachedObject = (AttachableObject*)Attached;
	AttachedObject->SetParentObject(this);
	AttachedObject->SetPickingIDBufferCached(PickingIDBufferCached);

	return Attached;
}

