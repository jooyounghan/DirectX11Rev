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
    if (ImGui::BeginMenu("Create New Asset"))
    {
        if (MenuItem("Material", NULL, false, true)) MaterialAssetCreated.Invoke();
        if (MenuItem("Retargeted Animation", NULL, false, true)) RetargetedAnimationCreated.Invoke();
        ImGui::EndMenu();
    }



}
