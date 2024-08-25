#pragma once
#include "AttachableObject.h"
 
struct ViewProjBufferData
{
	DirectX::XMMATRIX ViewProjMatrix;
	DirectX::XMMATRIX InvViewProjMatrix;
};

class Viewable : public AttachableObject
{
public:
	Viewable(GraphicsPipeline* GraphicsInstance, const UINT& WidthIn, const UINT& HeightIn);
	virtual ~Viewable();

protected:
	UINT Width = 0;
	UINT Height = 0;

protected:
	D3D11_VIEWPORT	Viewport;
	MakeGetter(Viewport);

public:
	float FovAngle	= 90.f;
	float NearZ		= 0.1f;
	float FarZ		= 1000.f;
	MakePointerGetter(FovAngle);
	MakePointerGetter(NearZ);
	MakePointerGetter(FarZ);

public:
	const DirectX::XMMATRIX GetViewMatrix();
	const DirectX::XMMATRIX GetProjectionMatrix();

protected:
	const DirectX::XMMATRIX GetPerspectiveViewMatrix();

public:
	UploadBuffer<ViewProjBufferData> ViewProjBuffer;

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;
	virtual void Resize(const UINT& WidthIn, const UINT& HeightIn);

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

