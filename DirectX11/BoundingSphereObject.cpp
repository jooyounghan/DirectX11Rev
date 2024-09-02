#include "BoundingSphereObject.h"
#include "AssetManager.h"
#include "Debugable.h"
#include "GraphicsPipeline.h"
#include "CollisionVisitor.h"
#include "IGuiModelVisitor.h"

using namespace std;
using namespace DirectX;

const char* BoundingSphereObject::BoundingSphereIdentifier = "Bounding Sphere";

BoundingSphereObject::BoundingSphereObject(GraphicsPipeline* GraphicsPipelineInstances)
	: ABoundingObject(GraphicsPipelineInstances), DescaledRadius(100.f)
{
	InitBoundingSphere(GraphicsPipelineInstances->GetDevice());
}

BoundingSphereObject::BoundingSphereObject(
	GraphicsPipeline* GraphicsPipelineInstances,
	const float& RadiusIn
)
	: ABoundingObject(GraphicsPipelineInstances),
	DescaledRadius(RadiusIn)
{
	InitBoundingSphere(GraphicsPipelineInstances->GetDevice());
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

	std::vector<DebugVertex>& VerticesIn = Result->Vertices;
	std::vector<uint16_t>& IndicesIn = Result->Indices;

	uint16_t DefaultSphereLevel = 5;

	for (uint16_t latitudeIdx = 0; latitudeIdx < DefaultSphereLevel; ++latitudeIdx)
	{
		const float& fLatitudeLow = DirectX::XM_PIDIV2 / DefaultSphereLevel * latitudeIdx;
		const float& fLatitudeHigh = DirectX::XM_PIDIV2 / DefaultSphereLevel * (latitudeIdx + 1);
		const float& fLatitudeLowTextureCord = (latitudeIdx / DefaultSphereLevel) / 2.f;
		const float& fLatitudeHighTextureCord = ((latitudeIdx + 1) / DefaultSphereLevel) / 2.f;

		const uint16_t& usLatitudeOffset = (uint16_t)VerticesIn.size();

		for (uint16_t longitudeIdx = 0; longitudeIdx <= (uint16_t)DefaultSphereLevel * 2; ++longitudeIdx)
		{
			const float& fLongitudeLow = DirectX::XM_2PI / (DefaultSphereLevel * 2.f) * longitudeIdx;
			const float& fLongitudeTextureCord = longitudeIdx / (DefaultSphereLevel * 2.f);

			VerticesIn.emplace_back(XMFLOAT3{ cosf(fLongitudeLow) * cosf(fLatitudeLow), sinf(fLatitudeLow), cosf(fLatitudeLow) * sinf(fLongitudeLow) });
			VerticesIn.emplace_back(XMFLOAT3{ cosf(fLongitudeLow) * cosf(fLatitudeHigh), sinf(fLatitudeHigh), cosf(fLatitudeHigh) * sinf(fLongitudeLow) });
			VerticesIn.emplace_back(XMFLOAT3{ cosf(fLongitudeLow) * cosf(-fLatitudeLow), sinf(-fLatitudeLow), cosf(-fLatitudeLow) * sinf(fLongitudeLow) });
			VerticesIn.emplace_back(XMFLOAT3{ cosf(fLongitudeLow) * cosf(-fLatitudeHigh), sinf(-fLatitudeHigh), cosf(-fLatitudeHigh) * sinf(fLongitudeLow) });
		}

		for (uint16_t longitudeIdx = 0; longitudeIdx < (uint16_t)DefaultSphereLevel * 2; ++longitudeIdx)
		{
			const uint16_t& usLongitudeOffset = 4 * longitudeIdx + usLatitudeOffset;
			IndicesIn.push_back(usLongitudeOffset + 0);
			IndicesIn.push_back(usLongitudeOffset + 1);
			IndicesIn.push_back(usLongitudeOffset + 4);
			IndicesIn.push_back(usLongitudeOffset + 4);
			IndicesIn.push_back(usLongitudeOffset + 1);
			IndicesIn.push_back(usLongitudeOffset + 5);

			IndicesIn.push_back(usLongitudeOffset + 3);
			IndicesIn.push_back(usLongitudeOffset + 2);
			IndicesIn.push_back(usLongitudeOffset + 7);
			IndicesIn.push_back(usLongitudeOffset + 7);
			IndicesIn.push_back(usLongitudeOffset + 2);
			IndicesIn.push_back(usLongitudeOffset + 6);
		}
	}

	Result->Initialize(DeviceIn);
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
