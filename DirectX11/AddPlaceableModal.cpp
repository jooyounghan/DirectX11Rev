#include "AddPlaceableModal.h"

#include "EnvironmentActor.h"
#include "SkeletalMeshObjectActor.h"
#include "StaticMeshObjectActor.h"

#include "MapAsset.h"

using namespace std;
using namespace ImGui;

const char* AddPlaceableModal::PlaceableItemIdentifiers[] =
{
    StaticMeshObjectActor::StaticMeshObjectActorKind.c_str(),
    SkeletalMeshObjectActor::SkeletalMeshObjectActorKind.c_str(),
    EnvironmentActor::EnvironmentActorKind.c_str(),
};


AddPlaceableModal::AddPlaceableModal(
    const string& ModalHeaderNameIn,
    EditorWorld* EditorWorldIn
)
    : AObjectManageModal(ModalHeaderNameIn, EditorWorldIn)
{
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
    static int ItemCurrentSelectedIdx = 0;
    ImGui::Combo("Select Class", &ItemCurrentSelectedIdx, PlaceableItemIdentifiers, IM_ARRAYSIZE(PlaceableItemIdentifiers));

    Separator();

    const char* SelectedObjectKind = PlaceableItemIdentifiers[ItemCurrentSelectedIdx];

    if (Button("OK", ImVec2(120, 0)))
    {
        if (SelectedObjectKind == StaticMeshObjectActor::StaticMeshObjectActorKind)
        {
            CurrentMapAssetCached->PlaceableAddHelper<StaticMeshObjectActor>(nullptr);
        }
        else if (SelectedObjectKind == SkeletalMeshObjectActor::SkeletalMeshObjectActorKind)
        {
            CurrentMapAssetCached->PlaceableAddHelper<SkeletalMeshObjectActor>(nullptr);
        }
        else if (SelectedObjectKind == EnvironmentActor::EnvironmentActorKind)
        {
            CurrentMapAssetCached->PlaceableAddHelper<EnvironmentActor>();
        }
        CloseCurrentPopup();
    }

    SetItemDefaultFocus();
    SameLine();
    if (Button("Cancel", ImVec2(120, 0))) { CloseCurrentPopup(); }
}
