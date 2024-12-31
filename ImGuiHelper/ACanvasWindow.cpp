#include "ACanvasWindow.h"

using namespace std;
using namespace ImGui;

ACanvasWindow::ACanvasWindow(const string& windowID)
	: AWindow(windowID)
{
	
	RegisterControl(&m_canvas);
}

void ACanvasWindow::RenderWindowImpl()
{
	Text("test");
	m_canvas.RenderControl();
}
