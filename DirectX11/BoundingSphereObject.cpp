#include "BoundingSphereObject.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

#include "AssetManager.h"
#include "Debugable.h"

#include "CollisionVisitor.h"
#include "IGuiModelVisitor.h"

#include "ModelHelper.h"

using namespace std;
using namespace DirectX;

const char* BoundingSphereObject::BoundingSphereIdentifier = "Bounding Sphere";

BoundingSphereObject::BoundingSphereObject()
	: ABoundingObject(), DescaledRadius(100.f)
{
	InitBoundingSphere(App::GGraphicPipeline->GetDevice());
}

BoundingSphereObject::BoundingSphereObject(const float& RadiusIn)
	: ABoundingObject(), DescaledRadius(RadiusIn)
{
	InitBoundingSphere(App::GGraphicPipeline->GetDevice());
}

BoundingSphereObject::~BoundingSphereObject()
{

}

void BoundingSphereObject::InitBoundingSphere(ID3D11Device* DeviceIn)
{
	static shared_ptr<Debugable> SphereDebugObject = CreateDebugSphereObject(DeviceIn);

	static size_t BoundingSphereCount = 0;
	BoundingSphereCount++;
	ObjectName = BoundingSphereIdentifier + to_string(BoundingSphereCount);
	AttachableKind = EAttachableObjectKind::BOUNDING_SPHERE_KIND;

	AutoZeroMemory(Center);

	DebugObject = SphereDebugObject.get();
}

shared_ptr<Debugable> BoundingSphereObject::CreateDebugSphereObject(ID3D11Device* DeviceIn)
{
	shared_ptr<Debugable> Result = make_shared<Debugable>(DeviceIn);

	std::vector<DirectX::XMFLOAT3>& VerticesIn = Result->Vertices;
	std::vector<uint16_t>& IndicesIn = Result->Indices;

	ModelHelper::CreateSphere<uint16_t>(5, &VerticesIn, nullptr, nullptr, &IndicesIn);

	Result->Initialize();
	return Result;
}

bool BoundingSphereObject::Intersect(Ray* RayIn, float& DistanceOut)
{
	return Intersects(RayIn->Origin, RayIn->Direction, DistanceOut);
}

bool BoundingSphereObject::AcceptCollision(ICollisionVisitor* CollisionVisitor)
{
	IsCollided = CollisionVisitor->Visit(this);
	SetCollisionColor();
	return IsCollided;
}


void BoundingSphereObject::Update(const float& DeltaTimeIn)
{
	Radius = DescaledRadius * RelativeScale.x;

	RelativeScale.x = Radius;
	RelativeScale.y = Radius;
	RelativeScale.z = Radius;
	AObject::Update(DeltaTimeIn);
	RelativeScale.x /= DescaledRadius;
	RelativeScale.y /= DescaledRadius;
	RelativeScale.z /= DescaledRadius;

	Center = GetAbsolutePosition();

	for (auto& ChildObject : AttachedChildrenObjects)
	{
		ChildObject->Update(DeltaTimeIn);
	}
}

void BoundingSphereObject::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void BoundingSphereObject::OnSerialize(FILE* FileIn)
{
	AObject::OnSerialize(FileIn);
	fwrite(&Radius, sizeof(float), 1, FileIn);
}

void BoundingSphereObject::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserialize(FileIn, AssetManagerIn);
	fread(&Radius, sizeof(float), 1, FileIn);
}
