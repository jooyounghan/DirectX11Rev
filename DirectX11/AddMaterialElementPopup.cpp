#include "AddMaterialElementPopup.h"

using namespace std;
using namespace ImGui;

AddMaterialElementPopup::AddMaterialElementPopup(const string& PopupIDIn)
	: APopup(PopupIDIn)
{
}

AddMaterialElementPopup::~AddMaterialElementPopup()
{
}

void AddMaterialElementPopup::RenderPopupMenu()
{
    if (MenuItem("Add BaseTexture Asset", NULL, false, true))
    {
        BaseTextureAdded.Invoke();
    }
    if (MenuItem("Add Float3", NULL, false, true))
    {
        Float3Added.Invoke();
    }
    if (MenuItem("Add Float", NULL, false, true))
    {
        FloatAdded.Invoke();
    }
}
