#include "ANodeEditorWindow.h"

using namespace std;
using namespace ImGui;

ANodeEditorWindow::ANodeEditorWindow(const string& windowID)
	: AWindow(windowID)
{
	RegisterInteractable(&m_canvas);
	m_canvas.AddNode(ImVec2(0.f, 0.f), ImVec2(20.f, 30.f));
}

void ANodeEditorWindow::RenderWindowImpl()
{
	m_canvas.RenderControl();
}
