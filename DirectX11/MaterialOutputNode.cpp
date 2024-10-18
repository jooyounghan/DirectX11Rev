#include "MaterialOutputNode.h"
#include "InputPort.h"

MaterialOutputNode::MaterialOutputNode(
	const ImVec2& CenterPositionIn, 
	const ImVec2& RectangleSizeIn, 
	std::shared_ptr<MaterialAsset> MaterialAssetIn
)
	: VariableOutputNode(CenterPositionIn, RectangleSizeIn, 8), MaterialAssetCached(MaterialAssetIn)
{
	size_t idx = 0;
	for (auto InputPortInstance : InputPorts)
	{
		switch (idx)
		{

		}
	}
}

MaterialOutputNode::~MaterialOutputNode()
{
}
