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
	for (size_t idx = 0; idx < 2; ++idx)
	{
		AddNode<0, FloatVariableNode>(10.f);
	}
	AddNode<0, StringVariableNode>(10.f);

	AddNode<0, FloatSumVariableNode>(10.f);
	AddNode<1, IBLMaterialAssetCreateFlowNode>(10.f);
	AdjustNodesLayout();
}

void CreateIBLMaterialWithNodeEditorWindow::RenderWindowImpl()
{
	ANodeEditorWindow::RenderWindowImpl();
}
