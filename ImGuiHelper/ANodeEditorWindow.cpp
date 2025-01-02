#include "ANodeEditorWindow.h"
#include "VariableNode.h"

using namespace std;
using namespace ImGui;

ANodeEditorWindow::ANodeEditorWindow(const string& windowID)
	: AWindow(windowID)
{
	m_canvas.RegisterToInteractionManager(this);
	for (size_t idx = 0; idx < 3; ++idx)
	{
		auto variableNode = new VariableNode<float, double, char, float, string>(
			"Test Node", ImVec2(100.f * idx, 0.f), 
			ImVec2(240.f, 120.f), 10.f, m_canvas.GetOriginPosition()
		);
		variableNode->AddToDrawElementManager(&m_canvas);
		variableNode->RegisterToInteractionManager(&m_canvas);
	}
}

void ANodeEditorWindow::RenderWindowImpl()
{
	m_canvas.RenderControl();
}
