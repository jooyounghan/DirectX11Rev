#include "BoundingSphere.h"
#include "AssetManager.h"
#include "Debugable.h"
#include "GraphicsPipeline.h"
#include "CollisionVisitor.h"
#include "IGuiModelVisitor.h"

using namespace std;
using namespace DirectX;

const char* BoundingSphere::BoundingSphereIdentifier = "Bounding Sphere";

BoundingSphere::BoundingSphere(GraphicsPipeline* GraphicsPipelineInstances)
	: ABoundingComponent(GraphicsPipelineInstances), Radius(100.f)
{
	InitBoundingSphere(GraphicsPipelineInstances->GetDevice());
}

BoundingSphere::BoundingSphere(
	GraphicsPipeline* GraphicsPipelineInstances,
	const float& RadiusIn
)
	: ABoundingComponent(GraphicsPipelineInstances),
	Radius(RadiusIn)
{
	InitBoundingSphere(GraphicsPipelineInstances->GetDevice());
}

BoundingSphere::~BoundingSphere()
{

}

void BoundingSphere::InitBoundingSphere(ID3D11Device* DeviceIn)
{
	static shared_ptr<Debugable> SphereDebugObject = CreateDebugSphereObject(DeviceIn);

	static size_t BoundingSphereCount = 0;
	BoundingSphereCount++;
	ObjectName = BoundingSphereIdentifier + to_string(BoundingSphereCount);
	AttachableKind = EAttachableObjectKind::BoundingSphereKind;

	AutoZeroMemory(Center);

	DebugObject = SphereDebugObject.get();
	DebugObject->UpdateColor(XMVectorSet(1.f, 0.f, 0.f, 1.f), DeviceContextCached);
}

shared_ptr<Debugable> BoundingSphere::CreateDebugSphereObject(ID3D11Device* DeviceIn)
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

			VerticesIn.emplace_back(SPosition3D{ cosf(fLongitudeLow) * cosf(fLatitudeLow), sinf(fLatitudeLow), cosf(fLatitudeLow) * sinf(fLongitudeLow) });
			VerticesIn.emplace_back(SPosition3D{ cosf(fLongitudeLow) * cosf(fLatitudeHigh), sinf(fLatitudeHigh), cosf(fLatitudeHigh) * sinf(fLongitudeLow) });
			VerticesIn.emplace_back(SPosition3D{ cosf(fLongitudeLow) * cosf(-fLatitudeLow), sinf(-fLatitudeLow), cosf(-fLatitudeLow) * sinf(fLongitudeLow) });
			VerticesIn.emplace_back(SPosition3D{ cosf(fLongitudeLow) * cosf(-fLatitudeHigh), sinf(-fLatitudeHigh), cosf(-fLatitudeHigh) * sinf(fLongitudeLow) });
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

bool BoundingSphere::Intersect(Ray* RayIn, float& DistanceOut)
{
	ScaledRadius = Radius * Scale.x; 

	const XMVECTOR ToCenter = RayIn->Origin - Center;

	float b = InnerProduct(RayIn->Direction, ToCenter);
	float c = InnerProduct(ToCenter, ToCenter) - ScaledRadius * ScaledRadius;

	float determinant = b * b - c;
	if (determinant < 0) 
	{ 
		return false; 
	}
	else
	{
		if (b > 0)
		{
			return false;
		}
		else
		{
			if (determinant == 0)
			{
				DistanceOut = -b;
			}
			else
			{
				float t1 = -b - sqrtf(b * b - c);
				float t2 = -b + sqrtf(b * b - c);
				DistanceOut = t1 < t2 ? t1 : t2;
			}
			return true;
		}
	}
}

bool BoundingSphere::AcceptCollision(ICollisionVisitor* CollisionVisitor)
{
	IsCollided = CollisionVisitor->Visit(this);
	return IsCollided;
}


void BoundingSphere::UpdateObject(const float& DeltaTimeIn)
{
	ABoundingComponent::UpdateObject(DeltaTimeIn);

	XMVECTOR Scaling;
	XMVECTOR RotationQuat;
	XMMatrixDecompose(&Scaling, &RotationQuat, &Center, GetTransformation());

}

void BoundingSphere::SetPropertyLength()
{
	Scale.x *= Radius;
	Scale.y *= Radius;
	Scale.z *= Radius;
}

void BoundingSphere::ResetPropertyLength()
{
	Scale.x /= Radius;
	Scale.y /= Radius;
	Scale.z /= Radius;
}

bool BoundingSphere::IsInsideOrOnPlane(const Plane& PlaneIn)
{
	XMVECTOR FromPlaneToSphere = Center - PlaneIn.Point;
	float DistanceFromPlaneToSphereCenter = XMVectorGetX(XMVector3Dot(FromPlaneToSphere, PlaneIn.Normal));
	return DistanceFromPlaneToSphereCenter >= -ScaledRadius;
}

bool BoundingSphere::IsOverlappedWithSphere(BoundingSphere* SphereIn)
{
	float Distance = XMVectorGetX(XMVector3Length(Center - SphereIn->Center));
	return Distance <= (ScaledRadius + SphereIn->ScaledRadius);
}

void BoundingSphere::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void BoundingSphere::OnSerialize(FILE* FileIn)
{
	AObject::OnSerialize(FileIn);
	fwrite(&Radius, sizeof(float), 1, FileIn);
}

void BoundingSphere::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserialize(FileIn, AssetManagerIn);
	fread(&Radius, sizeof(float), 1, FileIn);
}
