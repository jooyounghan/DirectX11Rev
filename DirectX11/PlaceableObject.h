#pragma once
#include "APlaceable.h"

class PlaceableObject : public APlaceable
{
public:
	PlaceableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn);
	virtual ~PlaceableObject();

public:
	virtual DirectX::XMVECTOR GetRotationQuat() const;

public:
	virtual DirectX::XMMATRIX GetScaleMatrix() const;
	virtual DirectX::XMMATRIX GetRotationMatrix() const;
	virtual DirectX::XMMATRIX GetTranslationMatrix() const;

public:
	virtual DirectX::XMMATRIX GetTransformation(const bool& IsIgnoreScale = false) const;

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;

public:
	virtual void AcceptRenderer(ARenderer* Renderer) override {};

public:
	virtual void AcceptGui(IGuiTopLevelVisitor* GuiVisitor);
	virtual void AcceptGui(IGuiLowLevelVisitor* GuiVisitor) = 0;
};

