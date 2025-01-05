#include "CreateIBLMaterialWithNodeEditorWindow.h"

#include "FloatVariableNode.h"
#include "ScratchTextureAssetNode.h"
#include "IBLMaterialAssetCreateFlowNode.h"

using namespace std;
using namespace ImGui;

CreateIBLMaterialWithNodeEditorWindow::CreateIBLMaterialWithNodeEditorWindow(
	AssetManager* assetManager, const std::string& windowID, bool* openFlag
)
	: ANodeEditorWindow(windowID, openFlag, 3), m_assetManagerCached(assetManager)
{	

}

void CreateIBLMaterialWithNodeEditorWindow::InitWindow()
{
	ANodeEditorWindow::InitWindow();

	for (size_t idx = 0; idx < 2; ++idx)
	{
		AddNode<FloatVariableNode>(0);
	}
	for (size_t idx = 0; idx < 4; ++idx)
	{
		AddNode<ScratchTextureAssetNode>(0, m_assetManagerCached);
	}

	AddNode<IBLMaterialAssetCreateFlowNode>(1);
}

void CreateIBLMaterialWithNodeEditorWindow::RenderWindowImpl()
{
	ANodeEditorWindow::RenderWindowImpl();
}
