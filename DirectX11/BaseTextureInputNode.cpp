#include "BaseTextureInputNode.h"
#include "BaseTextureOutputPort.h"
#include "AssetManager.h"
#include "AssetSelectHelper.h"


using namespace std;
using namespace ImGui;

BaseTextureInputNode::BaseTextureInputNode(
	const ImVec2& CenterPositionIn, 
	const ImVec2& RectangleSizeIn,
	AssetManager* AssetManagerIn,
	std::shared_ptr<BaseTextureAsset> BaseTextureAssetIn
)
	: VariableInputNode(CenterPositionIn, RectangleSizeIn), 
	AssetManagerCached(AssetManagerIn),
	BaseTextureAssetCached(BaseTextureAssetIn),
	ImageSizeX(ElementSize.x / 2.f - 2.f * NodePaddingSize),
	ImageSizeY(ElementSize.y - 2.f * NodePaddingSize),
	ImageSize(ImageSizeX > ImageSizeY ? ImVec2(ImageSizeY, ImageSizeY) : ImVec2(ImageSizeX, ImageSizeX)),
	ComboWidth(ElementSize.x - ImageSize.x - 3.f * NodePaddingSize)
{
	BaseTextureOutputPortCached = AddOutputPort<BaseTextureOutputPort>(ImVec2(NULL, NULL), 10.f, BaseTextureAssetCached);
}

BaseTextureInputNode::~BaseTextureInputNode()
{
}

void BaseTextureInputNode::AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn)
{
	NodeElement::AddToDrawList(OriginPosition, DrawListIn);

	const ImVec2 ImagePosition = ImVec2(
		LeftTopPosition.x + OriginPosition.x + NodePaddingSize,
		LeftTopPosition.y + OriginPosition.y + NodePaddingSize
	);
	const ImVec2 TextBoxPosition = ImVec2(
		ImagePosition.x + ImageSize.x + NodePaddingSize,
		ImagePosition.y + ImageSize.y / 2.f - GetTextLineHeight()
	);

	SetCursorScreenPos(ImagePosition);
	Image(BaseTextureAssetCached != nullptr ? BaseTextureAssetCached->GetSRV() : nullptr, ImageSize);

	SetCursorScreenPos(TextBoxPosition);
	const unordered_map<string, shared_ptr<BaseTextureAsset>>& BaseTextures = AssetManagerCached->GetManagingBasicTextures();
	shared_ptr<BaseTextureAsset> Result = AssetSelectHelper::SelectAsset(
		BaseTextures, BaseTextureAssetCached.get(), NodeID.c_str(), "Base Texture", "Choose BaseTexuture Asset", NULL, ComboWidth);

	if (Result != nullptr)
	{
		BaseTextureAssetCached = Result;

		// Reconnect
		BaseTextureOutputPortCached->SetBaseTextureAssetCached(BaseTextureAssetCached);
		BaseTextureOutputPortCached->Connect(BaseTextureOutputPortCached->GetConnectedPort());
	}
}

