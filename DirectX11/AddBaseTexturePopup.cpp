#include "AddBaseTexturePopup.h"

using namespace std;
using namespace ImGui;

AddBaseTexturePopup::AddBaseTexturePopup(const string& PopupIDIn)
	: APopup(PopupIDIn)
{
}

AddBaseTexturePopup::~AddBaseTexturePopup()
{
}

void AddBaseTexturePopup::RenderPopupMenu()
{
    if (MenuItem("Add BaseTexture Asset", NULL, false, true))
    {
        BaseTextureAdded.Invoke();
    }
}
