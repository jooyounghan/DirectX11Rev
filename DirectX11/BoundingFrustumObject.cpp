#include "BoundingFrustumObject.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "DefineUtility.h"

#include "Viewable.h"
#include "Debugable.h"

#include "CollisionVisitor.h"
#include "IGuiModelVisitor.h"

using namespace std;
using namespace DirectX;

string BoundingFrustumObject::BoundingFrustumKind = "Bounding Frustum";

BoundingFrustumObject::BoundingFrustumObject(Viewable* ViewableInstance)
	: ABoundingObject(BoundingFrustumObject::BoundingFrustumKind), ViewableCached(ViewableInstance)
{
    static shared_ptr<Debugable> FrustumDebugObject = CreateDebugFrustumObject(App::GGraphicPipeline->GetDevice());

    static size_t BoundingFrustumCount = 0;
    BoundingFrustumCount++;
    ObjectName = BoundingFrustumObject::BoundingFrustumKind + to_string(BoundingFrustumCount);

    DebugObject = FrustumDebugObject.get();
}

BoundingFrustumObject::~BoundingFrustumObject()
{

}

std::shared_ptr<Debugable> BoundingFrustumObject::CreateDebugFrustumObject(ID3D11Device* DeviceIn)
{
    shared_ptr<Debugable> Result = make_shared<Debugable>(DeviceIn);

    std::vector<XMFLOAT3>& VerticesIn = Result->Vertices;
    std::vector<uint16_t>& IndicesIn = Result->Indices;

    VerticesIn.emplace_back(XMFLOAT3{ -1.0f, -1.0f, 0.0f });
    VerticesIn.emplace_back(XMFLOAT3{ 1.0f, -1.0f, 0.0f });
    VerticesIn.emplace_back(XMFLOAT3{ 1.0f,  1.0f, 0.0f });
    VerticesIn.emplace_back(XMFLOAT3{ -1.0f,  1.0f, 0.0f });
    VerticesIn.emplace_back(XMFLOAT3{ -1.0f, -1.0f,  1.0f });
    VerticesIn.emplace_back(XMFLOAT3{ 1.0f, -1.0f,  1.0f });
    VerticesIn.emplace_back(XMFLOAT3{ 1.0f,  1.0f,  1.0f });
    VerticesIn.emplace_back(XMFLOAT3{ -1.0f,  1.0f,  1.0f });

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

    Result->Initialize();
    return Result;
}

bool BoundingFrustumObject::Intersect(Ray* RayIn, float& DistanceOut)
{
    return Intersects(RayIn->Origin, RayIn->Direction, DistanceOut);
}

bool BoundingFrustumObject::AcceptCollision(ICollisionVisitor* CollisionVisitor)
{
    IsCollided = CollisionVisitor->Visit(this);
    SetCollisionColor();
    return IsCollided;
}

void BoundingFrustumObject::Update(const float& DeltaTimeIn)
{
	if (ViewableCached != nullptr)
	{
        TransformationMatrix TempTransformation;

        XMVECTOR vRelativePos = XMLoadFloat3(&RelativePosition);

        TempTransformation.TransfomationMat =
            XMMatrixInverse(nullptr, ViewableCached->GetProjectionMatrix()) *
                XMMatrixLookToLH(vRelativePos, Direction::GDefaultForward, Direction::GDefaultUp) *
                ViewableCached->GetTransformation();

        TempTransformation.InvTransfomationMat = XMMatrixInverse(nullptr, TempTransformation.TransfomationMat);
        TempTransformation.TransfomationMat = XMMatrixTranspose(TempTransformation.TransfomationMat);

        TransformationBuffer->SetStagedData(TempTransformation);

        const D3D11_VIEWPORT& Viewport = ViewableCached->GetViewport();
        const float AspectRatio = Viewport.Width / Viewport.Height;

        XMVECTOR Translation;
        XMVECTOR Rotation;
        XMVECTOR Scaling;
        const XMMATRIX Transformation = ViewableCached->GetTransformation();       
        XMMatrixDecompose(&Scaling, &Rotation, &Translation, Transformation);

        XMStoreFloat3(&Origin, Translation);
        XMStoreFloat4(&Orientation, Rotation);

        XMVECTOR CurrentForward = XMVector3Rotate(Direction::GDefaultForward, Rotation);

        const XMVECTOR NearPosition = ViewableCached->NearZ * CurrentForward;
        const XMVECTOR FarPosition = ViewableCached->FarZ * CurrentForward;

        Near = ViewableCached->NearZ;
        Far = ViewableCached->FarZ;

        TopSlope = tanf(XMConvertToRadians(ViewableCached->FovAngle) * 0.5f);
        BottomSlope = -TopSlope;

        RightSlope = TopSlope * AspectRatio;
        LeftSlope = -RightSlope;

        for (auto& ChildObject : AttachedChildrenObjects)
        {
            ChildObject->Update(DeltaTimeIn);
        }

	}
}

void BoundingFrustumObject::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
    GuiVisitor->Visit(this);
}

void BoundingFrustumObject::OnSerialize(FILE* FileIn)
{
}

void BoundingFrustumObject::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
}
