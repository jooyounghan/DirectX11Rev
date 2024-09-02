#include "AController.h"
#include "DefineUtility.h"

#include "APawn.h"

AController::AController()
{
}


void AController::MoveForward(const float& DeltaTime)
{
	if (PossesdPawn != nullptr)
	{
		PossesdPawn->MoveForward(DeltaTime * MovingSpeed);
	}
}

void AController::MoveBackward(const float& DeltaTime)
{
	if (PossesdPawn != nullptr)
	{
		PossesdPawn->MoveForward(-DeltaTime * MovingSpeed);
	}
}

void AController::MoveRight(const float& DeltaTime)
{
	if (PossesdPawn != nullptr)
	{
		PossesdPawn->MoveRight(DeltaTime * MovingSpeed);
	}
}

void AController::MoveLeft(const float& DeltaTime)
{
	if (PossesdPawn != nullptr)
	{
		PossesdPawn->MoveRight(-DeltaTime * MovingSpeed);
	}
}
