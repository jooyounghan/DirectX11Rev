#include "DeletePlaceableModal.h"
#include "APlaceableObject.h"
#include "EditorWorld.h"
#include "MapAsset.h"

using namespace std;
using namespace ImGui;

DeletePlaceableModal::DeletePlaceableModal(
    const string& ModalHeaderNameIn,
    EditorWorld* EditorWorldIn
)
    : AObjectManageModal(ModalHeaderNameIn, EditorWorldIn)
{
}

DeletePlaceableModal::~DeletePlaceableModal()
{
}

bool DeletePlaceableModal::ModalCondition()
{
    AAttachableObject* SelectedAttached = EditorWorldCached->GetSelectedAttached();
    return SelectedAttached == nullptr && IsKeyPressed(ImGuiKey::ImGuiKey_Delete) && IsWindowFocused();
}

void DeletePlaceableModal::RenderModal()
{
    APlaceableObject* SelectedPlaceable = EditorWorldCached->GetSelectedPlaceable();

    if (SelectedPlaceable != nullptr)
    {
        Text("Are you sure for deleting %s ?", SelectedPlaceable->GetObjectName().c_str());
        Separator();

        if (Button("OK", ImVec2(120, 0)))
        {
            if (CurrentMapAssetCached != nullptr)
            {
                CurrentMapAssetCached->PlaceableDeleteHelper(SelectedPlaceable);
                EditorWorldCached->SetSelectedPlaceable(nullptr);
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
