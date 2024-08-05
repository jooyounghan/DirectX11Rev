#include "BoundingFrustum.h"
#include "Viewable.h"
#include "GlobalVariable.h"
#include "DefineUtility.h"
#include "CollisionVisitor.h"

BoundingFrustum::BoundingFrustum(
	GraphicsPipeline* GraphicsPipelineInstances,
	AssetManager* AssetManagerInstance,
	Viewable* ViewableInstance
)
	: ABoundingComponent(GraphicsPipelineInstances, AssetManagerInstance), ViewableCached(ViewableInstance)
{
    AutoZeroMemory(TopFace);
    AutoZeroMemory(BottomFace);
    AutoZeroMemory(RightFace);
    AutoZeroMemory(LeftFace);
    AutoZeroMemory(FarFace);
    AutoZeroMemory(NearFace);
}

BoundingFrustum::~BoundingFrustum()
{
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
        const float HalfVSide = ViewableCached->FarZ * tanf(ViewableCached->FovAngle * 0.5f);
        const float HalfHSide = HalfVSide * AspectRatio;

        const XMVECTOR RotationQuat = ViewableCached->GetRotationQuat();
        XMVECTOR CurrentForward = XMVector3Rotate(Direction::GDefaultForward, RotationQuat);
        XMVECTOR CurrentUp = XMVector3Rotate(Direction::GDefaultUp, RotationQuat);
        XMVECTOR CurrentRight = XMVector3Rotate(Direction::GDefaultRight, RotationQuat);

        const XMVECTOR NearPosition = ViewableCached->NearZ * CurrentForward;
        const XMVECTOR FarPosition = ViewableCached->FarZ * CurrentForward;

        const SPosition4D Postion = ViewableCached->Position;
        const XMVECTOR XMVPosition = XMVectorSet(Position.x, Position.y, Position.z, Position.w);

        NearFace = { XMVPosition + NearPosition, CurrentForward };
        FarFace = { XMVPosition + FarPosition, -CurrentForward };
        
        RightFace = { XMVPosition, XMVector3Normalize(XMVector3Cross(FarPosition - CurrentRight * HalfHSide, CurrentUp)) };
        LeftFace = { XMVPosition, XMVector3Normalize(XMVector3Cross(CurrentUp,FarPosition + CurrentRight * HalfHSide)) };
        TopFace = { XMVPosition, XMVector3Normalize(XMVector3Cross(CurrentRight, FarPosition - CurrentUp * HalfVSide)) };
        BottomFace = { XMVPosition, XMVector3Normalize(XMVector3Cross(FarPosition + CurrentUp * HalfVSide, CurrentRight)) };
	}
}
