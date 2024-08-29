#pragma once
#include "AObject.h"
#include "StaticAssertHelper.h"
#include "IIntersectable.h"

#include <list>
#include <memory>

class APlaceableObject;

enum EAttachableObjectKind : int
{
	AttachableNone,
	MeshObjectKind,
	BoundingSphereKind,
	OrientedBoundingBoxKind,
	NormalCameraKind,
};

class AAttachableObject : public AObject
{

public:
	AAttachableObject(GraphicsPipeline* GraphicsPipelineInstance);
	virtual~AAttachableObject();

protected:
	EAttachableObjectKind AttachableKind = EAttachableObjectKind::AttachableNone;
	MakeSetterGetter(AttachableKind);

protected:
	AObject* ParentObject = nullptr;
	MakeSetterGetter(ParentObject);

protected:
	std::list<std::unique_ptr<AAttachableObject>> AttachedChildrenObjects;
	MakeGetter(AttachedChildrenObjects);

protected:
	ID3D11Buffer* PickingIDBufferCached = nullptr;
	MakeSetterGetter(PickingIDBufferCached);

public:
	virtual DirectX::XMVECTOR GetRotationQuat() const override;

public:
	virtual DirectX::XMFLOAT3 GetAbsolutePosition() const override;
	virtual DirectX::XMFLOAT3 GetAbsoluteAngle() const override;
	virtual DirectX::XMFLOAT3 GetAbsoluteScale() const override;

public:
	virtual DirectX::XMMATRIX GetTransformation() const override;

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptRenderer(ARenderer* Renderer) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	template<typename Attachment, typename ...Args>
	Attachment* AddAttachedObject(Args... args);

private:
	template<typename Attachment>
	void AddToIntersectables(Attachment* AttachedObjet);

private:
	void AddIntersectableToRootPlaceable(APlaceableObject* RootPlaceable, IIntersectable* Intersectable);

public:
	void RemoveAttachedObject(AAttachableObject* AttachedObjectIn);

private:
	void RemoveFromIntersectables(AAttachableObject* AttachedObjectIn);

public:
	virtual void OnSerialize(FILE* FileIn) = 0;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;
};

template<typename Attachment, typename ...Args>
inline Attachment* AAttachableObject::AddAttachedObject(Args ...args)
{
	static_assert(std::is_base_of<AAttachableObject, Attachment>::value, DerivedCondition(AAttachableObject));

	AttachedChildrenObjects.emplace_back(std::make_unique<Attachment>(args...));
	Attachment* Attached = (Attachment*)AttachedChildrenObjects.back().get();

	AddToIntersectables(Attached);

	AAttachableObject* AttachedObject = (AAttachableObject*)Attached;
	AttachedObject->SetParentObject(this);
	AttachedObject->SetPickingIDBufferCached(PickingIDBufferCached);

	return Attached;
}

template<typename Attachment>
inline void AAttachableObject::AddToIntersectables(Attachment* AttachedObjet)
{
	if (std::is_base_of<IIntersectable, Attachment>::value)
	{
		AObject* CurrentParent = ParentObject;
		AAttachableObject* AttachableParent = nullptr;
		APlaceableObject* PlaceableParent = nullptr;

		AttachableParent = dynamic_cast<AAttachableObject*>(ParentObject);
		while (AttachableParent != nullptr)
		{
			CurrentParent = AttachableParent->ParentObject;
			AttachableParent = dynamic_cast<AAttachableObject*>(AttachableParent->ParentObject);
		}

		PlaceableParent = (APlaceableObject*)CurrentParent;
		AddIntersectableToRootPlaceable(PlaceableParent, (IIntersectable*)AttachedObjet);
	}
}
