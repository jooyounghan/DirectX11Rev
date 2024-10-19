#include "BaseTextureOutputPort.h"
#include "AssetVariableInputPort.h"

using namespace std;

BaseTextureOutputPort::BaseTextureOutputPort(
	const ImVec2& CenterPositionIn, 
	const float& RadiusSizeIn,
	shared_ptr<BaseTextureAsset> BaseTextureAssetIn
)
	: VariableOutputPort(CenterPositionIn, RadiusSizeIn), BaseTextureAssetCached(BaseTextureAssetIn)
{
}

BaseTextureOutputPort::~BaseTextureOutputPort()
{
}

bool BaseTextureOutputPort::IsConnectable(InputPort* PortIn)
{
	AssetVariableInputPort* AssetVariableInputNodeCached = dynamic_cast<AssetVariableInputPort*>(PortIn);
	if (AssetVariableInputNodeCached != nullptr)
	{
		if (AssetVariableInputNodeCached->GetPortAssetType() == EAssetType::BaseTexture)
		{
			return true;
		}
	}
	return false;
}
