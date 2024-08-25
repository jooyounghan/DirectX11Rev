#include "BoundingFrustum.h"

#include "GraphicsPipeline.h"
#include "GlobalVariable.h"
#include "DefineUtility.h"

#include "Viewable.h"
#include "Debugable.h"

#include "CollisionVisitor.h"
#include "IGuiModelVisitor.h"

using namespace std;

const char* BoundingFrustum::BoundingFrustumIdentifier = "Bounding Frustum";

BoundingFrustum::BoundingFrustum(
	GraphicsPipeline* GraphicsPipelineInstances,
	Viewable* ViewableInstance
)
	: ABoundingComponent(GraphicsPipelineInstances), ViewableCached(ViewableInstance)
{
    static shared_ptr<Debugable> FrustumDebugObject = CreateDebugFrustumObject(GraphicsPipelineInstances->GetDevice());

    static size_t BoundingFrustumCount = 0;
    BoundingFrustumCount++;
    ObjectName = BoundingFrustumIdentifier + to_string(BoundingFrustumCount);

    AutoZeroArrayMemory(FrustumPlanes);
    AutoZeroArrayMemory(FrustumEdgeAxises);

    DebugObject = FrustumDebugObject.get();
}

BoundingFrustum::~BoundingFrustum()
{

}

std::shared_ptr<Debugable> BoundingFrustum::CreateDebugFrustumObject(ID3D11Device* DeviceIn)
{
    shared_ptr<Debugable> Result = make_shared<Debugable>(DeviceIn);

    std::vector<DebugVertex>& VerticesIn = Result->Vertices;
    std::vector<uint16_t>& IndicesIn = Result->Indices;

    VerticesIn.emplace_back(SPosition3D{ -1.0f, -1.0f, 0.0f });
    VerticesIn.emplace_back(SPosition3D{ 1.0f, -1.0f, 0.0f });
    VerticesIn.emplace_back(SPosition3D{ 1.0f,  1.0f, 0.0f });
    VerticesIn.emplace_back(SPosition3D{ -1.0f,  1.0f, 0.0f });
    VerticesIn.emplace_back(SPosition3D{ -1.0f, -1.0f,  1.0f });
    VerticesIn.emplace_back(SPosition3D{ 1.0f, -1.0f,  1.0f });
    VerticesIn.emplace_back(SPosition3D{ 1.0f,  1.0f,  1.0f });
    VerticesIn.emplace_back(SPosition3D{ -1.0f,  1.0f,  1.0f });

    IndicesIn = std::vector<uint16_t>{
        // Front face
        0, 2, 1, 0, 3, 2,
        // Back face
        4, 5, 6, 4, 6, 7,
        // Bottom face
        0, 1, 5, 0, 5, 4,
        // Top face
        3, 7, 6, 3, 6, 2,
        // Left face
        0, 4, 7, 0, 7, 3,
        // Right face
        1, 2, 6, 1, 6, 5
    };

    Result->Initialize(DeviceIn);
    return Result;


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
	if (ViewableCached != nullptr)
	{
        TransformationMatrix TempTransformation;

        TempTransformation.TransfomationMat =
            XMMatrixInverse(nullptr, ViewableCached->GetProjectionMatrix()) *
            XMMatrixLookToLH(Position.Position, Direction::GDefaultForward, Direction::GDefaultUp) *
            ViewableCached->GetTransformation();

        TempTransformation.InvTransfomationMat = XMMatrixInverse(nullptr, TempTransformation.TransfomationMat);
        TempTransformation.TransfomationMat = XMMatrixTranspose(TempTransformation.TransfomationMat);

        TransformationBuffer.Upload(DeviceContextCached, TempTransformation);


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

        const SPosition4D& ViewablePostion = ViewableCached->Position;
        XMVECTOR XMVPosition = XMVectorSet(ViewablePostion.x, ViewablePostion.y, ViewablePostion.z, ViewablePostion.w);
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

        SetCollisionColor();

        for (auto& ChildObject : AttachedChildrenObjects)
        {
            ChildObject->UpdateObject(DeltaTimeIn);
        }

	}
}

void BoundingFrustum::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
    GuiVisitor->Visit(this);
}

void BoundingFrustum::OnSerialize(FILE* FileIn)
{
}

void BoundingFrustum::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
}
