#include "ANodeEditorWindow.h"


using namespace std;
using namespace ImGui;

ANodeEditorWindow::ANodeEditorWindow(const string& windowID, bool* openFlag)
	: AWindow(windowID, false, openFlag)
{
	m_canvas.RegisterToInteractionManager(this);
}

void ANodeEditorWindow::RenderWindowImpl()
{
	m_canvas.RenderControl();
}
