#include "DeleteAttachableModal.h"
#include "AAttachableObject.h"
#include "APlaceableObject.h"
#include "MapAsset.h"

using namespace std;
using namespace ImGui;

DeleteAttachableModal::DeleteAttachableModal(const string& ModalHeaderNameIn)
	: AObjectManageModal(ModalHeaderNameIn)
{
}

DeleteAttachableModal::~DeleteAttachableModal()
{
}

bool DeleteAttachableModal::ModalCondition()
{
	return CurrentMapAssetCached != nullptr &&
        CurrentMapAssetCached->GetSelectedAttachable() != nullptr &&
        IsKeyPressed(ImGuiKey::ImGuiKey_Delete) && IsWindowFocused();
}

void DeleteAttachableModal::RenderModal()
{
	AAttachableObject* SelectedAttached = CurrentMapAssetCached->GetSelectedAttachable();

    if (SelectedAttached != nullptr)
    {
        Text("Are you sure for Deleting %s ?", SelectedAttached->GetObjectName().c_str());
        Separator();

        if (Button("OK", ImVec2(120, 0)))
        {
            APlaceableObject* SelectedPlaceableObject = CurrentMapAssetCached->GetSelectedPlaceable();
            if (SelectedPlaceableObject != nullptr)
            {
                SelectedPlaceableObject->RemoveAttachedObject(SelectedAttached);
                CurrentMapAssetCached->SetSelectedAttachable(nullptr);
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
