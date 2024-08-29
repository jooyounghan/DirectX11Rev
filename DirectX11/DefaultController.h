#pragma once

#include "AController.h"

class DefaultController : public AController
{
public:
	DefaultController();
	virtual ~DefaultController();

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void ConsumeKeyInput(EKeyAction KeyAction, EKeyInput KeyInput) override;
	virtual void ConsumeMouseDelta(const float& DeltaNDCX, const float DeltaNDCY) override;
};

