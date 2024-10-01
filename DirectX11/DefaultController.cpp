#include "DefaultController.h"

#include "GlobalVariable.h"
#include "InputEventManager.h"

#include "APawn.h"

DefaultController::DefaultController()
	: AController()
{
	KeyEvent& WPressed = App::GInputEventManager->GetKeyEvent(EKeyAction::KEY_PRESSED, EKeyInput::KEY_W);
	KeyEvent& SPressed = App::GInputEventManager->GetKeyEvent(EKeyAction::KEY_PRESSED, EKeyInput::KEY_S);
	KeyEvent& APressed = App::GInputEventManager->GetKeyEvent(EKeyAction::KEY_PRESSED, EKeyInput::KEY_A);
	KeyEvent& DPressed = App::GInputEventManager->GetKeyEvent(EKeyAction::KEY_PRESSED, EKeyInput::KEY_D);

	KeyEvent& CtrlDown = App::GInputEventManager->GetKeyEvent(EKeyAction::KEY_DOWN, EKeyInput::KEY_CTRL);
	KeyEvent& CtrlUp = App::GInputEventManager->GetKeyEvent(EKeyAction::KEY_UP, EKeyInput::KEY_CTRL);

	MouseEvent& MouseMove = App::GInputEventManager->GetMouseEvnet(EMouseAction::MOUSE_MOVE);

	WPressed.AddEventHandler(&AController::MoveForward, this);
	SPressed.AddEventHandler(&AController::MoveBackward, this);
	APressed.AddEventHandler(&AController::MoveLeft, this);
	DPressed.AddEventHandler(&AController::MoveRight, this);

	CtrlDown.AddEventHandler(&DefaultController::StartViewChanging, this);
	CtrlUp.AddEventHandler(&DefaultController::FinishViewChanging, this);

	MouseMove.AddEventHandler(&DefaultController::ChangeView, this);
}

void DefaultController::StartViewChanging(const float& DeltaTime) 
{ 
	EnableViewChange = true && (PossesdPawn != nullptr); 
	IsNdcRefSaved = false;
}

void DefaultController::FinishViewChanging(const float& DeltaTime) 
{ 
	EnableViewChange = false; 
}

void DefaultController::ChangeView(const float& NdcX, const float& NdcY)
{
	if (!IsNdcRefSaved)
	{
		NdcXRef = NdcX;
		NdcYRef = NdcY;
		IsNdcRefSaved = true;
		return;
	}

	if (EnableViewChange)
	{
		const float& DeltaNdcX = NdcXRef - NdcX;
		const float& DeltaNdcY = NdcYRef - NdcY;

		if (PossesdPawn != nullptr)
		{
			PossesdPawn->RotateYaw(-DeltaNdcX * AngleSpeed);
			PossesdPawn->RotatePitch(-DeltaNdcY * AngleSpeed);
		}

		NdcXRef = NdcX;
		NdcYRef = NdcY;
	}
}
