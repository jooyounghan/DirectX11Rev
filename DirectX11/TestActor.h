#pragma once
#include "PlaceableObject.h"
#include "HeaderHelper.h"

class TestActor : public PlaceableObject
{
public:
	TestActor(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn);
	virtual ~TestActor();

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) {};
};

