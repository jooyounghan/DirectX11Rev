#include "OBBObject.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "DefineUtility.h"

#include "Debugable.h"
#include "CollisionVisitor.h"
#include "IGuiModelVisitor.h"

#include "ModelHelper.h"

#include <limits>

using namespace std;
using namespace DirectX;

string OBBObject::BoundingOBBKind = "Bounding OBB";

OBBObject::OBBObject()
	: ABoundingObject(OBBObject::BoundingOBBKind)
{
	InitOBB(App::GGraphicPipeline->GetDevice());

	DescaledExtents.x = 100.f;
	DescaledExtents.y = 100.f;
	DescaledExtents.z = 100.f;
}

OBBObject::OBBObject(
	const float& HalfXIn, 
	const float& HalfYIn,
	const float& HalfZIn
)
	: ABoundingObject(OBBObject::BoundingOBBKind)
{
	InitOBB(App::GGraphicPipeline->GetDevice());
	DescaledExtents.x = HalfXIn;
	DescaledExtents.y = HalfYIn;
	DescaledExtents.z = HalfZIn;
}

OBBObject::~OBBObject() {}

void OBBObject::InitOBB(ID3D11Device* DeviceIn)
{
	static shared_ptr<Debugable> OBBDebugObject = CreateDebugBoxObject(DeviceIn);

	static size_t BoundingOBBCount = 0;
	BoundingOBBCount++;

	ObjectName = OBBObject::BoundingOBBKind + to_string(BoundingOBBCount);

	DebugObject = OBBDebugObject.get();
}

shared_ptr<Debugable> OBBObject::CreateDebugBoxObject(ID3D11Device* DeviceIn)
{
	shared_ptr<Debugable> Result = make_shared<Debugable>(DeviceIn);

	std::vector<XMFLOAT3>& VerticesIn = Result->Vertices;
	std::vector<uint16_t>& IndicesIn = Result->Indices;

	ModelHelper::CreateCube(&VerticesIn, nullptr, nullptr, &IndicesIn, false);

	Result->Initialize();
	return Result;
}

bool OBBObject::Intersect(Ray* RayIn, float& DistanceOut)
{
	return Intersects(RayIn->Origin, RayIn->Direction, DistanceOut);
}

bool OBBObject::AcceptCollision(ICollisionVisitor* CollisionVisitor)
{
	IsCollided = CollisionVisitor->Visit(this);
	SetCollisionColor();
	return IsCollided;
}

void OBBObject::Update(const float& DeltaTimeIn)
{
	Extents.x = DescaledExtents.x * RelativeScale.x;
	Extents.y = DescaledExtents.y * RelativeScale.y;
	Extents.z = DescaledExtents.z * RelativeScale.z;

	RelativeScale.x = Extents.x;
	RelativeScale.y = Extents.y;
	RelativeScale.z = Extents.z;
	AObject::Update(DeltaTimeIn);
	RelativeScale.x /= DescaledExtents.x;
	RelativeScale.y /= DescaledExtents.y;
	RelativeScale.z /= DescaledExtents.z;

	XMVECTOR Scaling;
	XMVECTOR Rotation;
	XMVECTOR Translation;
	XMMatrixDecompose(&Scaling, &Rotation, &Translation, GetTransformation());

	XMStoreFloat3(&Center, Translation);
	XMStoreFloat4(&Orientation, Rotation);

	for (auto& ChildObject : AttachedChildrenObjects)
	{
		ChildObject->Update(DeltaTimeIn);
	}
}

void OBBObject::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void OBBObject::OnSerialize(FILE* FileIn)
{
	AObject::OnSerialize(FileIn);
	fwrite(&DescaledExtents, sizeof(XMFLOAT3), 1, FileIn);
}

void OBBObject::OnDeserialize(FILE* FileIn)
{
	AObject::OnDeserialize(FileIn);
	fread(&DescaledExtents, sizeof(XMFLOAT3), 1, FileIn);
}
