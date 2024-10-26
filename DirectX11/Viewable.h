#pragma once
#include "AAttachableObject.h"
 
struct ViewProjBufferData
{
	DirectX::XMMATRIX ViewProjMatrix;
	DirectX::XMMATRIX InvViewProjMatrix;
	DirectX::XMFLOAT3 ViewPosition;
	float Dummy;
};

class Viewable : public AAttachableObject
{
public:
	Viewable(
		MapAsset* MapAssetInstance, 
		const UINT& WidthIn, const UINT& HeightIn,
		const std::string& AttachableKindIn
	);
	virtual ~Viewable();

protected:
	D3D11_VIEWPORT	Viewport;
	MakeGetter(Viewport);

public:
	float FovAngle	= 80.f;
	float NearZ		= 0.1f;
	float FarZ		= 10000.f;
	MakePointerGetter(FovAngle);
	MakePointerGetter(NearZ);
	MakePointerGetter(FarZ);

public:
	const DirectX::XMMATRIX GetViewMatrix();
	const DirectX::XMMATRIX GetProjectionMatrix();

protected:
	const DirectX::XMMATRIX GetPerspectiveViewMatrix();

protected:
	UploadBuffer<ViewProjBufferData>* ViewProjBuffer;
	MakeGetter(ViewProjBuffer);

public:
	virtual void Update(const float& DeltaTimeIn) override;
	virtual void Resize(const UINT& WidthIn, const UINT& HeightIn);

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;

public:
	virtual void Render() override;
};

