#include "DeletePlaceableModal.h"
#include "MapOutlinerWindow.h"
#include "APlaceableObject.h"
#include "MapAsset.h"

using namespace std;
using namespace ImGui;

DeletePlaceableModal::DeletePlaceableModal(MapOutlinerWindow* MapOutlinerWindowIn, const string& ModalHeaderNameIn)
    : AModal(ModalHeaderNameIn), MapOutlinerWindowCached(MapOutlinerWindowIn)
{
}

DeletePlaceableModal::~DeletePlaceableModal()
{
}

bool DeletePlaceableModal::ModalCondition()
{
    return MapAssetCached != nullptr && 
        MapOutlinerWindowCached->GetSelectedPlaceable() == nullptr &&
        IsKeyPressed(ImGuiKey::ImGuiKey_Delete) && IsWindowFocused();
}

void DeletePlaceableModal::RenderModal()
{
    APlaceableObject* SelectedPlaceable = MapOutlinerWindowCached->GetSelectedPlaceable();

    if (SelectedPlaceable != nullptr)
    {
        Text("Are you sure for deleting %s ?", SelectedPlaceable->GetObjectName().c_str());
        Separator();

        if (Button("OK", ImVec2(120, 0)))
        {
            if (MapAssetCached != nullptr)
            {
                MapAssetCached->PlaceableDeleteHelper(SelectedPlaceable);
                MapOutlinerWindowCached->SetSelectedPlaceable(nullptr);
            }
            CloseCurrentPopup();
        }
        SetItemDefaultFocus();
        SameLine();
        if (Button("Cancel", ImVec2(120, 0)))
        {
            CloseCurrentPopup();
        }
    }
    else
    {
        CloseCurrentPopup();
    }
}
