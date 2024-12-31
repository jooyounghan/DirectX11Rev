#include "ANodeEditorWindow.h"

using namespace std;
using namespace ImGui;

ANodeEditorWindow::ANodeEditorWindow(const string& windowID)
	: AWindow(windowID)
{
	RegisterInteractable(&m_canvas);
	m_canvas.AddNode(ImVec2(0.f, 0.f), ImVec2(240.f, 120.f));
}

void ANodeEditorWindow::RenderWindowImpl()
{
	m_canvas.RenderControl();
}
