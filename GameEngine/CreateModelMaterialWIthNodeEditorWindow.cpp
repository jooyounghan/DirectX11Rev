#include "CreateModelMaterialWIthNodeEditorWindow.h"

#include "AssetWriterPathNode.h"
#include "StringVariableNode.h"
#include "BaseTextureAssetVariableNode.h"
#include "Float3VariableNode.h"
#include "FloatVariableNode.h"
#include "ModelMaterialAssetCreateFlowNode.h"

CreateModelMaterialWIthNodeEditorWindow::CreateModelMaterialWIthNodeEditorWindow(
	ID3D11Device* const* deviceAddress, ID3D11DeviceContext* const* deviceContextAddress, 
	AssetManager* assetManager, const std::string& windowID, bool* openFlag
)
	: ANodeEditorWindow(windowID, openFlag, 3), m_assetManagerCached(assetManager)
{
	AddNode<AssetWriterPathNode>(0, m_assetManagerCached);
	AddNode<StringVariableNode>(0);
	for (size_t idx = 0; idx < 8; ++idx)
	{
		AddNode<BaseTextureAssetVariableNode>(0, m_assetManagerCached);
	}

	AddNode<FloatVariableNode>(1);
	AddNode<Float3VariableNode>(1);

	AddNode<ModelMaterialAssetCreateFlowNode>(1, deviceAddress, deviceContextAddress, assetManager);
}

void CreateModelMaterialWIthNodeEditorWindow::RenderWindowImpl()
{
	ANodeEditorWindow::RenderWindowImpl();
}
