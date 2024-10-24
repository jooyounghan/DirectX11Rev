#include "AddPlaceableModal.h"

#include "EnvironmentActor.h"
#include "SkeletalMeshObjectActor.h"
#include "StaticMeshObjectActor.h"

#include "MapAsset.h"

using namespace std;
using namespace ImGui;

AddPlaceableModal::AddPlaceableModal(
    const string& ModalHeaderNameIn,
    EditorWorld* EditorWorldIn
)
    : AObjectManageModal(ModalHeaderNameIn, EditorWorldIn)
{
    PlaceableItemIdentifiers =
    {
        StaticMeshObjectActor::StaticMeshObjectActorKind,
        SkeletalMeshObjectActor::SkeletalMeshObjectActorKind,
        EnvironmentActor::EnvironmentActorKind,
    };
}

AddPlaceableModal::~AddPlaceableModal()
{
}

bool AddPlaceableModal::ModalCondition()
{
    SameLine();
	const char* SmallButtonText = "Add Placeable +";
	float ButtonWidth = CalcTextSize(SmallButtonText).x;
	SetCursorPosX(GetCursorPosX() + GetContentRegionAvail().x - ButtonWidth);
	return SmallButton(SmallButtonText);
}

void AddPlaceableModal::RenderModal()
{
    static string SelectedPlaceableKind;
    if (ImGui::BeginCombo("Select Placeable Object", SelectedPlaceableKind.empty() ? "Select Placeable Object" : SelectedPlaceableKind.c_str()))
    {
        for (const string& PlaceableItemIdentifier : PlaceableItemIdentifiers)
        {
            if (ImGui::Selectable(PlaceableItemIdentifier.c_str()))
            {
                SelectedPlaceableKind = PlaceableItemIdentifier;
            }
        }
        ImGui::EndCombo();
    }

    Separator();

    if (Button("OK", ImVec2(120, 0)))
    {
        if (SelectedPlaceableKind == StaticMeshObjectActor::StaticMeshObjectActorKind)
        {
            CurrentMapAssetCached->PlaceableAddHelper<StaticMeshObjectActor>(nullptr);
        }
        else if (SelectedPlaceableKind == SkeletalMeshObjectActor::SkeletalMeshObjectActorKind)
        {
            CurrentMapAssetCached->PlaceableAddHelper<SkeletalMeshObjectActor>(nullptr);
        }
        else if (SelectedPlaceableKind == EnvironmentActor::EnvironmentActorKind)
        {
            CurrentMapAssetCached->PlaceableAddHelper<EnvironmentActor>();
        }
        CloseCurrentPopup();
    }

    SetItemDefaultFocus();
    SameLine();
    if (Button("Cancel", ImVec2(120, 0))) { CloseCurrentPopup(); }
}
