#include "DefaultController.h"
#include "InputEventManager.h"
#include "GlobalVariable.h"

DefaultController::DefaultController()
	: AController()
{
	KeyEvent& WPressed = App::InputEventManagerCached->GetKeyEvent(EKeyAction::KEY_PRESSED, EKeyInput::KEY_W);
	KeyEvent& SPressed = App::InputEventManagerCached->GetKeyEvent(EKeyAction::KEY_PRESSED, EKeyInput::KEY_S);
	KeyEvent& APressed = App::InputEventManagerCached->GetKeyEvent(EKeyAction::KEY_PRESSED, EKeyInput::KEY_A);
	KeyEvent& DPressed = App::InputEventManagerCached->GetKeyEvent(EKeyAction::KEY_PRESSED, EKeyInput::KEY_D);

	WPressed.AddEventHandler(&AController::MoveForward, this);
	SPressed.AddEventHandler(&AController::MoveBackward, this);
	APressed.AddEventHandler(&AController::MoveLeft, this);
	DPressed.AddEventHandler(&AController::MoveRight, this);
}
