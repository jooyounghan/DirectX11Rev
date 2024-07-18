#include "Viewable.h"
#include "DefineUtility.h"
#include "GlobalVariable.h"

using namespace std;
using namespace DirectX;

Viewable::Viewable(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn, const UINT& WidthIn, const UINT& HeightIn)
	: Object(DeviceIn, DeviceContextIn), ViewProjBuffer(DeviceIn)
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

XMMATRIX Viewable::GetPerspectiveViewMatrix()
{
	const XMVECTOR RotationQuat = GetRotationQuat();
	XMVECTOR CurrentForward = XMVector3Rotate(Direction::GDefaultForward, RotationQuat);
	XMVECTOR CurrentUp = XMVector3Rotate(Direction::GDefaultUp, RotationQuat);

	return XMMatrixLookToLH(Position.Position, CurrentForward, CurrentUp) *
		XMMatrixPerspectiveFovLH(XMConvertToRadians(FovAngle), static_cast<float>(Width) / Height, NearZ, FarZ);
}

void Viewable::UpdateView()
{
	ViewProjBufferData TempViewProj;
	TempViewProj.ViewProjMatrix = GetPerspectiveViewMatrix();
	TempViewProj.InvViewProjMatrix = XMMatrixInverse(nullptr, TempViewProj.ViewProjMatrix);
	TempViewProj.ViewProjMatrix = XMMatrixTranspose(TempViewProj.ViewProjMatrix);

	ViewProjBuffer.Upload(DeviceContextCached, TempViewProj);
}
