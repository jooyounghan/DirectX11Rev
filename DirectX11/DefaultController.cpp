#include "DefaultController.h"

DefaultController::DefaultController()
{
}

DefaultController::~DefaultController()
{
}

void DefaultController::Update(const float& DeltaTimeIn)
{
}

void DefaultController::ConsumeKeyInput(EKeyAction KeyAction, EKeyInput KeyInput)
{
	switch (KeyAction)
	{
	case EKeyAction::KEY_DOWN:
		break;
	case EKeyAction::KEY_UP:
		break;
	default:
		break;
	}
}

void DefaultController::ConsumeMouseDelta(const float& DeltaNDCX, const float DeltaNDCY)
{

}
