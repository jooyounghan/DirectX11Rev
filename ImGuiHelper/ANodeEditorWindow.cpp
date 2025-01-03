#include "ANodeEditorWindow.h"
#include "VariableNode.h"

using namespace std;
using namespace ImGui;

ANodeEditorWindow::ANodeEditorWindow(const string& windowID)
	: AWindow(windowID)
{
	m_canvas.RegisterToInteractionManager(this);

	// Example
	auto variableNode1 = new VariableNode<float, double, char, float, string>(
		"Test Node", ImVec2(100.f, 100.f), 10.f, m_canvas.GetOriginPosition()
	);
	variableNode1->AddToDrawElementManager(&m_canvas);
	variableNode1->RegisterToInteractionManager(&m_canvas);

	auto variableNode2 = new VariableNode<double>(
		"Double", ImVec2(240.f, 120.f), 10.f, m_canvas.GetOriginPosition()
	);
	variableNode2->AddToDrawElementManager(&m_canvas);
	variableNode2->RegisterToInteractionManager(&m_canvas);

	auto variableNode3 = new VariableNode<char>(
		"Char", ImVec2(240.f, 120.f), 10.f, m_canvas.GetOriginPosition()
	);
	variableNode3->AddToDrawElementManager(&m_canvas);
	variableNode3->RegisterToInteractionManager(&m_canvas);

	auto variableNode4 = new VariableNode<float>(
		"Float", ImVec2(240.f, 120.f), 10.f, m_canvas.GetOriginPosition()
	);
	variableNode4->AddToDrawElementManager(&m_canvas);
	variableNode4->RegisterToInteractionManager(&m_canvas);

	auto variableNode5 = new VariableNode<std::string>(
		"string", ImVec2(240.f, 120.f), 10.f, m_canvas.GetOriginPosition()
	);
	variableNode5->AddToDrawElementManager(&m_canvas);
	variableNode5->RegisterToInteractionManager(&m_canvas);
}

void ANodeEditorWindow::RenderWindowImpl()
{
	m_canvas.RenderControl();
}
