#pragma once

#include "ISerializable.h"
#include "IUpdatable.h"
#include "DefineKey.h"
#include "HeaderHelper.h"

class APawn;

class AController : public IUpdatable
{
public:
	AController();

protected:
	float MovingSpeed = 1.f;
	float AngleSpeed = 360.f;
	MakeGetter(MovingSpeed);
	MakePointerGetter(MovingSpeed);
	MakeGetter(AngleSpeed);
	MakePointerGetter(AngleSpeed);

protected:
	APawn* PossesdPawn = nullptr;
	MakeSetterGetter(PossesdPawn);

public:
	virtual void Update(const float& DeltaTimeIn) = 0;


public:
	void MoveForward(const float& DeltaTime);
	void MoveBackward(const float& DeltaTime);
	void MoveRight(const float& DeltaTime);
	void MoveLeft(const float& DeltaTime);

public:
	void RotatePitch();
	void RotateRoll();
	void RotateYaw();

public:
	virtual void ConsumeKeyInput(EKeyAction KeyAction, EKeyInput KeyInput) = 0;
	virtual void ConsumeMouseDelta(const float& DeltaNDCX, const float DeltaNDCY) = 0;
};

