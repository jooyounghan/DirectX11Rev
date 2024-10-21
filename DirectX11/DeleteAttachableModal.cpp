#include "DeleteAttachableModal.h"
#include "AAttachableObject.h"
#include "APlaceableObject.h"
#include "EditorWorld.h"

using namespace std;
using namespace ImGui;

DeleteAttachableModal::DeleteAttachableModal(
	const string& ModalHeaderNameIn,
	EditorWorld* EditorWorldIn
)
	: AObjectManageModal(ModalHeaderNameIn, EditorWorldIn)
{
}

DeleteAttachableModal::~DeleteAttachableModal()
{
}

bool DeleteAttachableModal::ModalCondition()
{
	return IsKeyPressed(ImGuiKey::ImGuiKey_Delete) && IsWindowFocused();
}

void DeleteAttachableModal::RenderModal()
{
	AAttachableObject* SelectedAttached = EditorWorldCached->GetSelectedAttached();

    if (SelectedAttached != nullptr)
    {
        Text("Are you sure for Deleting %s ?", SelectedAttached->GetObjectName().c_str());
        Separator();

        if (Button("OK", ImVec2(120, 0)))
        {
            APlaceableObject* SelectedPlaceableObject = EditorWorldCached->GetSelectedPlaceable();
            if (SelectedPlaceableObject != nullptr)
            {
                SelectedPlaceableObject->RemoveAttachedObject(SelectedAttached);
                EditorWorldCached->SetSelectedAttached(nullptr);
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
