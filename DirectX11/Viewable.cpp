#include "Viewable.h"
#include "DefineUtility.h"
#include "GlobalVariable.h"

#include "ARenderer.h"
#include "IGuiModelVisitor.h"

using namespace std;
using namespace DirectX;

Viewable::Viewable(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn, const UINT& WidthIn, const UINT& HeightIn)
	: AttachableObject(DeviceIn, DeviceContextIn), ViewProjBuffer(DeviceIn)
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
}

void Viewable::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
}

