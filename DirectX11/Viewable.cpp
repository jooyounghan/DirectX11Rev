#include "Viewable.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "DefineUtility.h"

#include "IGuiModelVisitor.h"

#include "UploadableBufferManager.h"

using namespace std;
using namespace DirectX;

Viewable::Viewable(
	const UINT& WidthIn, const UINT& HeightIn,
	const string& AttachableKindIn
)
	: AAttachableObject(AttachableKindIn)
{
	ViewProjBuffer = App::GUploadableBufferManager->CreateUploadableBuffer<UploadBuffer<ViewProjBufferData>>();
	Resize(WidthIn, HeightIn);
}

Viewable::~Viewable()
{
}

void Viewable::Resize(const UINT& WidthIn, const UINT& HeightIn)
{
	AutoZeroMemory(Viewport);
	Viewport.Width = static_cast<FLOAT>(WidthIn);
	Viewport.Height = static_cast<FLOAT>(HeightIn);
	Viewport.TopLeftX = 0.f;
	Viewport.TopLeftY = 0.f;
	Viewport.MinDepth = 0.f;
	Viewport.MaxDepth = 1.f;
}

const XMMATRIX Viewable::GetViewMatrix()
{
	const XMVECTOR RotationQuat = GetRotationQuat();
	XMVECTOR CurrentForward = XMVector3Rotate(Direction::GDefaultForward, RotationQuat);
	XMVECTOR CurrentUp = XMVector3Rotate(Direction::GDefaultUp, RotationQuat);

	XMFLOAT3 AbsPosition = GetAbsolutePosition();
	return XMMatrixLookToLH(XMLoadFloat3(&AbsPosition), CurrentForward, CurrentUp);
}

const XMMATRIX Viewable::GetProjectionMatrix()
{
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(FovAngle), static_cast<float>(Viewport.Width) / Viewport.Height, NearZ, FarZ);
}

const XMMATRIX Viewable::GetPerspectiveViewMatrix()
{
	return GetViewMatrix() * GetProjectionMatrix();
}

void Viewable::Update(const float& DeltaTimeIn)
{
	AAttachableObject::Update(DeltaTimeIn);

	ViewProjBufferData TempViewProj;
	TempViewProj.ViewProjMatrix = GetPerspectiveViewMatrix();
	TempViewProj.InvViewProjMatrix = XMMatrixInverse(nullptr, TempViewProj.ViewProjMatrix);
	TempViewProj.ViewProjMatrix = XMMatrixTranspose(TempViewProj.ViewProjMatrix);
	TempViewProj.ViewPosition = GetAbsolutePosition();

	ViewProjBuffer->SetStagedData(TempViewProj);
}


void Viewable::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void Viewable::OnSerialize(FILE* FileIn)
{
	AObject::OnSerialize(FileIn);

	fwrite(&Viewport, sizeof(D3D11_VIEWPORT), 1, FileIn);
	fwrite(&FovAngle, sizeof(float), 1, FileIn);
	fwrite(&NearZ, sizeof(float), 1, FileIn);
	fwrite(&FarZ, sizeof(float), 1, FileIn);
}

void Viewable::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserialize(FileIn, AssetManagerIn);
	fread(&Viewport, sizeof(D3D11_VIEWPORT), 1, FileIn);
	fread(&FovAngle, sizeof(float), 1, FileIn);
	fread(&NearZ, sizeof(float), 1, FileIn);
	fread(&FarZ, sizeof(float), 1, FileIn);
}

void Viewable::Render(MapAsset* MapAssetIn)
{
	AAttachableObject::Render(MapAssetIn);
}
