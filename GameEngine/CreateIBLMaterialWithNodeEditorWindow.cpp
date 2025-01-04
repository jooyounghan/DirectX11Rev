#include "CreateIBLMaterialWithNodeEditorWindow.h"

#include "FloatSumVariableNode.h"
#include "FloatVariableNode.h"
#include "StringVariableNode.h"
#include "IBLMaterialAssetCreateFlowNode.h"

using namespace std;
using namespace ImGui;

CreateIBLMaterialWithNodeEditorWindow::CreateIBLMaterialWithNodeEditorWindow(
	const std::string& windowID, bool* openFlag
)
	: ANodeEditorWindow(windowID, openFlag)
{	
	for (size_t idx = 0; idx < 4; ++idx)
	{
		auto variableNode = new FloatVariableNode(
			ImVec2(100.f, 100.f), 10.f, m_canvas.GetOriginPosition()
		);
		variableNode->AddToDrawElementManager(&m_canvas);
		variableNode->RegisterToInteractionManager(&m_canvas);
	}

	auto floatSum = new  FloatSumVariableNode(
		ImVec2(100.f, 100.f), 10.f, m_canvas.GetOriginPosition()
	);
	floatSum->AddToDrawElementManager(&m_canvas);
	floatSum->RegisterToInteractionManager(&m_canvas);

	auto flowNode1 = new IBLMaterialAssetCreateFlowNode(
		ImVec2(240.f, 240.f), 10.f, m_canvas.GetOriginPosition()
	);
	flowNode1->AddToDrawElementManager(&m_canvas);
	flowNode1->RegisterToInteractionManager(&m_canvas);

}

void CreateIBLMaterialWithNodeEditorWindow::RenderWindowImpl()
{
	ANodeEditorWindow::RenderWindowImpl();
}
