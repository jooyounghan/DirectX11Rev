#include "Viewable.h"


#include "GraphicsPipeline.h"
#include "DefineUtility.h"
#include "GlobalVariable.h"

#include "ARenderer.h"
#include "IGuiModelVisitor.h"

using namespace std;
using namespace DirectX;

Viewable::Viewable(GraphicsPipeline* GraphicsInstance, const UINT& WidthIn, const UINT& HeightIn)
	: AttachableObject(GraphicsInstance), ViewProjBuffer(GraphicsInstance->GetDevice())
{
	Resize(WidthIn, HeightIn);
}

Viewable::~Viewable()
{
}

void Viewable::Resize(const UINT& WidthIn, const UINT& HeightIn)
{
	Width = WidthIn;
	Height = HeightIn;

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

	return XMMatrixLookToLH(Position.Position, CurrentForward, CurrentUp);
}

const XMMATRIX Viewable::GetProjectionMatrix()
{
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(FovAngle), static_cast<float>(Width) / Height, NearZ, FarZ);
}

const XMMATRIX Viewable::GetPerspectiveViewMatrix()
{
	return GetViewMatrix() * GetProjectionMatrix();
}

void Viewable::UpdateObject(const float& DeltaTimeIn)
{
	AttachableObject::UpdateObject(DeltaTimeIn);

	ViewProjBufferData TempViewProj;
	TempViewProj.ViewProjMatrix = GetPerspectiveViewMatrix();
	TempViewProj.InvViewProjMatrix = XMMatrixInverse(nullptr, TempViewProj.ViewProjMatrix);
	TempViewProj.ViewProjMatrix = XMMatrixTranspose(TempViewProj.ViewProjMatrix);

	ViewProjBuffer.Upload(DeviceContextCached, TempViewProj);
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