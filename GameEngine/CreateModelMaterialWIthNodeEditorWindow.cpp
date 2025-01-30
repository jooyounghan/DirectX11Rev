#include "CreateModelMaterialWIthNodeEditorWindow.h"

#include "AssetManager.h"
#include "AssetWriterPathNode.h"
#include "StringVariableNode.h"
#include "BaseTextureAssetVariableNode.h"
#include "Float3VariableNode.h"
#include "FloatVariableNode.h"
#include "ModelMaterialAssetCreateFlowNode.h"

CreateModelMaterialWIthNodeEditorWindow::CreateModelMaterialWIthNodeEditorWindow(
	ID3D11Device* const* deviceAddress, ID3D11DeviceContext* const* deviceContextAddress, 
	const std::string& windowID, bool* openFlag
)
	: ANodeEditorWindow(windowID, openFlag, 3)
{
	AddNode<AssetWriterPathNode>(0);
	AddNode<StringVariableNode>(0);
	for (size_t idx = 0; idx < 8; ++idx)
	{
		AddNode<BaseTextureAssetVariableNode>(0);
	}

	AddNode<FloatVariableNode>(1);
	AddNode<Float3VariableNode>(1);

	AddNode<ModelMaterialAssetCreateFlowNode>(1, deviceAddress, deviceContextAddress);
}

void CreateModelMaterialWIthNodeEditorWindow::RenderWindowImpl()
{
	ANodeEditorWindow::RenderWindowImpl();
}
