#include "AController.h"
#include "DefineUtility.h"

#include "APawn.h"

AController::AController()
{
	AutoZeroArrayMemory(KeyPressedStates);
}

void AController::MoveForward(const float& DeltaTime)
{
	if (PossesdPawn != nullptr)
	{
		PossesdPawn->RelativePosition.z += MovingSpeed * DeltaTime;
	}
}

void AController::MoveBackward(const float& DeltaTime)
{
	if (PossesdPawn != nullptr)
	{
		PossesdPawn->RelativePosition.z -= MovingSpeed * DeltaTime;
	}
}

void AController::MoveRight(const float& DeltaTime)
{
	if (PossesdPawn != nullptr)
	{
		PossesdPawn->RelativePosition.x += MovingSpeed * DeltaTime;
	}
}

void AController::MoveLeft(const float& DeltaTime)
{
	if (PossesdPawn != nullptr)
	{
		PossesdPawn->RelativePosition.x -= MovingSpeed * DeltaTime;
	}
}

void AController::RotatePitch()
{
}

void AController::RotateRoll()
{
}

void AController::RotateYaw()
{
}
