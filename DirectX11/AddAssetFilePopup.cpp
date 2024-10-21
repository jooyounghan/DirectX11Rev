#include "AddAssetFilePopup.h"

using namespace std;
using namespace ImGui;

AddAssetFilePopup::AddAssetFilePopup(const std::string& PopupIDIn)
	: APopup(PopupIDIn)
{
}

AddAssetFilePopup::~AddAssetFilePopup()
{
}

void AddAssetFilePopup::RenderPopupMenu()
{
    if (MenuItem("Create New Asset", NULL, false, true))
    {
        AssetCreated.Invoke();
    }
}
