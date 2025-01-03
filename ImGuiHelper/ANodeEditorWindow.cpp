#include "ANodeEditorWindow.h"
#include "FloatVariableNode.h"
#include "StringVariableNode.h"

using namespace std;
using namespace ImGui;

ANodeEditorWindow::ANodeEditorWindow(const string& windowID)
	: AWindow(windowID)
{
	m_canvas.RegisterToInteractionManager(this);

	// Example
	for (size_t idx = 0; idx < 2; ++idx)
	{
		auto variableNode1 = new FloatVariableNode(
			ImVec2(100.f, 100.f), 10.f, m_canvas.GetOriginPosition()
		);
		variableNode1->AddToDrawElementManager(&m_canvas);
		variableNode1->RegisterToInteractionManager(&m_canvas);

		auto variableNode3 = new StringVariableNode(
			ImVec2(240.f, 120.f), 10.f, m_canvas.GetOriginPosition()
		);
		variableNode3->AddToDrawElementManager(&m_canvas);
		variableNode3->RegisterToInteractionManager(&m_canvas);

	}
}

void ANodeEditorWindow::RenderWindowImpl()
{
	m_canvas.RenderControl();
}
