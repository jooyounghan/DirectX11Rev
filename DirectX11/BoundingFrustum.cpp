#include "BoundingFrustum.h"
#include "Viewable.h"
#include "GlobalVariable.h"
#include "DefineUtility.h"
#include "CollisionVisitor.h"
#include "IGuiModelVisitor.h"

using namespace std;

BoundingFrustum::BoundingFrustum(
	GraphicsPipeline* GraphicsPipelineInstances,
	AssetManager* AssetManagerInstance,
	Viewable* ViewableInstance
)
	: ABoundingComponent(GraphicsPipelineInstances, AssetManagerInstance), ViewableCached(ViewableInstance)
{
    static size_t BoundingFrustumCount = 0;
    BoundingFrustumCount++;
    ObjectName = "Bounding Frustum " + to_string(BoundingFrustumCount);

    AutoZeroArrayMemory(FrustumPlanes);
    AutoZeroArrayMemory(FrustumEdgeAxises);
}

BoundingFrustum::~BoundingFrustum()
{
}

const Plane& BoundingFrustum::GetFrustumPlane(Direction::EFrstumDirection DirectionIn)
{
    return FrustumPlanes[DirectionIn];
}

const XMVECTOR& BoundingFrustum::GetFrustumEdgeAxis(Direction::EFrustumEdgeAxis EdgeAxisIn)
{
    return FrustumEdgeAxises[EdgeAxisIn];
}

bool BoundingFrustum::Intersect(Ray* RayIn, float& DistanceOut)
{
    return false;
}

bool BoundingFrustum::AcceptCollision(ICollisionVisitor* CollisionVisitor)
{
    return CollisionVisitor->Visit(this);
}

void BoundingFrustum::UpdateObject(const float& DeltaTimeIn)
{
    ABoundingComponent::UpdateObject(DeltaTimeIn);

	if (ViewableCached != nullptr)
	{
        const D3D11_VIEWPORT& Viewport = ViewableCached->GetViewport();
        const float AspectRatio = Viewport.Width / Viewport.Height;
        const float HalfVSide = ViewableCached->FarZ * tanf(XMConvertToRadians(ViewableCached->FovAngle) * 0.5f);
        const float HalfHSide = HalfVSide * AspectRatio;

        const XMVECTOR RotationQuat = ViewableCached->GetRotationQuat();
        const XMMATRIX Transformation = ViewableCached->GetTransformation();

        XMVECTOR CurrentForward = XMVector3Rotate(Direction::GDefaultForward, RotationQuat);
        XMVECTOR CurrentUp = XMVector3Rotate(Direction::GDefaultUp, RotationQuat);
        XMVECTOR CurrentRight = XMVector3Rotate(Direction::GDefaultRight, RotationQuat);

        const XMVECTOR NearPosition = ViewableCached->NearZ * CurrentForward;
        const XMVECTOR FarPosition = ViewableCached->FarZ * CurrentForward;

        const SPosition4D& Postion = ViewableCached->Position;
        XMVECTOR XMVPosition = XMVectorSet(Position.x, Position.y, Position.z, Position.w);
        XMVPosition = XMVector3Transform(XMVPosition, Transformation);

        FrustumPlanes[Direction::EFrstumDirection::FrustumNear] = { XMVPosition + NearPosition, XMVector3Normalize(CurrentForward) };
        FrustumPlanes[Direction::EFrstumDirection::FrustumFar] = { XMVPosition + FarPosition, -XMVector3Normalize(CurrentForward) };
        FrustumPlanes[Direction::EFrstumDirection::FrustumRight] = { XMVPosition, XMVector3Normalize(XMVector3Cross(FarPosition + CurrentRight * HalfHSide, CurrentUp)) };
        FrustumPlanes[Direction::EFrstumDirection::FrustumLeft] = { XMVPosition, XMVector3Normalize(XMVector3Cross(FarPosition - CurrentRight * HalfHSide, -CurrentUp)) };
        FrustumPlanes[Direction::EFrstumDirection::FrustumTop] = { XMVPosition, XMVector3Normalize(XMVector3Cross(FarPosition + CurrentUp * HalfVSide, -CurrentRight)) };
        FrustumPlanes[Direction::EFrstumDirection::FrustumBottom] = { XMVPosition, XMVector3Normalize(XMVector3Cross(FarPosition - CurrentUp * HalfVSide, CurrentRight)) };
     
        FrustumEdgeAxises[Direction::LeftTop] = FarPosition - CurrentRight * HalfHSide + CurrentUp * HalfVSide;
        FrustumEdgeAxises[Direction::LeftBottom] = FarPosition - CurrentRight * HalfHSide - CurrentUp * HalfVSide;
        FrustumEdgeAxises[Direction::RightTop] = FarPosition + CurrentRight * HalfHSide + CurrentUp * HalfVSide;
        FrustumEdgeAxises[Direction::RightBottm] = FarPosition + CurrentRight * HalfHSide - CurrentUp * HalfVSide;
	}
}

void BoundingFrustum::SetPropertyLength()
{
}

void BoundingFrustum::ResetPropertyLength()
{
}

void BoundingFrustum::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
    GuiVisitor->Visit(this);
}
