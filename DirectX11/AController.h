#pragma once

#include "ISerializable.h"
#include "IUpdatable.h"
#include "HeaderHelper.h"

class APawn;

class AController
{
public:
	AController();

protected:
	float MovingSpeed = 100.f;
	float AngleSpeed = 360.f;
	MakeGetter(MovingSpeed);
	MakePointerGetter(MovingSpeed);
	MakeGetter(AngleSpeed);
	MakePointerGetter(AngleSpeed);

protected:
	APawn* PossesdPawn = nullptr;
	MakeSetterGetter(PossesdPawn);

public:
	void MoveForward(const float& DeltaTime);
	void MoveBackward(const float& DeltaTime);
	void MoveRight(const float& DeltaTime);
	void MoveLeft(const float& DeltaTime);
};

