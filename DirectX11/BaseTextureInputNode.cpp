#include "BaseTextureInputNode.h"
#include "InputPort.h"

BaseTextureInputNode::BaseTextureInputNode(
	const ImVec2& CenterPositionIn, 
	const ImVec2& RectangleSizeIn, 
	std::shared_ptr<BaseTextureAsset> BaseTextureAssetIn
)
	: VariableInputNode(CenterPositionIn, RectangleSizeIn), BaseTextureAssetCached(BaseTextureAssetIn)
{
}

BaseTextureInputNode::~BaseTextureInputNode()
{
}
