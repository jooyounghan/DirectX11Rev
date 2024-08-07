#pragma once
#include "Attachable.h"

class AttachableObject : public AAttachable
{
public:
	AttachableObject();
	virtual~AttachableObject();

public:
	virtual void Render(PSOObject* PSOObjectIn) override {};
	virtual void UpdateObject(const float& DeltaTimeIn) override;

public:
	virtual void AcceptGui(IGuiTopLevelVisitor* GuiVisitor);
};

