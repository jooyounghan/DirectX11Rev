#include "DeleteAttachableModal.h"

#include "MapOutlinerWindow.h"
#include "AAttachableObject.h"
#include "APlaceableObject.h"
#include "MapAsset.h"

using namespace std;
using namespace ImGui;

DeleteAttachableModal::DeleteAttachableModal(MapOutlinerWindow* MapOutlinerWindowIn, const string& ModalHeaderNameIn)
	: AModal(ModalHeaderNameIn), MapOutlinerWindowCached(MapOutlinerWindowIn)
{
}

DeleteAttachableModal::~DeleteAttachableModal()
{
}

bool DeleteAttachableModal::ModalCondition()
{
	return MapOutlinerWindowCached->GetSelectedAttachable() != nullptr &&
        IsKeyPressed(ImGuiKey::ImGuiKey_Delete) && IsWindowFocused();
}

void DeleteAttachableModal::RenderModal()
{
	AAttachableObject* SelectedAttached = MapOutlinerWindowCached->GetSelectedAttachable();

    if (SelectedAttached != nullptr)
    {
        Text("Are you sure for Deleting %s ?", SelectedAttached->GetObjectName().c_str());
        Separator();

        if (Button("OK", ImVec2(120, 0)))
        {
            APlaceableObject* SelectedPlaceableObject = MapOutlinerWindowCached->GetSelectedPlaceable();
            if (SelectedPlaceableObject != nullptr)
            {
                SelectedPlaceableObject->RemoveAttachedObject(SelectedAttached);
                MapOutlinerWindowCached->SetSelectedAttachable(nullptr);
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
