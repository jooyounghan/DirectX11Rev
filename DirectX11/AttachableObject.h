#pragma once
#include "Attachable.h"

class AttachableObject : public AAttachable
{
public:
	AttachableObject();
	virtual~AttachableObject();

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;
	virtual void Render(PSOObject* PSOObjectIn) override {};
};

