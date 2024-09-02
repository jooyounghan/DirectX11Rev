#pragma once

#include "AController.h"

class DefaultController : public AController
{
public:
	DefaultController();

public:
	bool EnableViewChange = false;
	bool IsNdcRefSaved = false;
	float NdcXRef = 0.f;
	float NdcYRef = 0.f;

public:
	void StartViewChanging(const float& DeltaTime);
	void FinishViewChanging(const float& DeltaTime);

public:
	void ChangeView(const float& NdcX, const float& NdcY);
};

