#pragma once
#include "APlaceable.h"

class RelativePlaceableObject : public AAttachable, public APlaceable
{
public:
	RelativePlaceableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn);
	virtual ~RelativePlaceableObject();

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
	virtual void Render(PSOObject* PSOObjectIn) override {};

public:
	virtual void AcceptGui(IGuiTopLevelVisitor* GuiVisitor);
	virtual void AcceptGui(IGuiLowLevelVisitor* GuiVisitor) = 0;
};

