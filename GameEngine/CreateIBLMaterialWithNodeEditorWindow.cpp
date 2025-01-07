#include "CreateIBLMaterialWithNodeEditorWindow.h"

#include "AssetWriterPathNode.h"
#include "StringVariableNode.h"
#include "ScratchTextureAssetVariableNode.h"
#include "FloatVariableNode.h"
#include "IBLMaterialAssetCreateFlowNode.h"

using namespace std;
using namespace ImGui;

CreateIBLMaterialWithNodeEditorWindow::CreateIBLMaterialWithNodeEditorWindow(
	AssetManager* assetManager, const std::string& windowID, bool* openFlag
)
	: ANodeEditorWindow(windowID, openFlag, 3), m_assetManagerCached(assetManager)
{	
	AddNode<AssetWriterPathNode>(0, m_assetManagerCached);
	AddNode<StringVariableNode>(0);
	for (size_t idx = 0; idx < 4; ++idx)
	{
		AddNode<ScratchTextureAssetVariableNode>(0, m_assetManagerCached);
	}

	for (size_t idx = 0; idx < 2; ++idx)
	{
		AddNode<FloatVariableNode>(1);
	}

	AddNode<IBLMaterialAssetCreateFlowNode>(1);
}


void CreateIBLMaterialWithNodeEditorWindow::RenderWindowImpl()
{
	ANodeEditorWindow::RenderWindowImpl();
}
