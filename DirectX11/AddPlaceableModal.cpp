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
    const char* ItemIdentifiers[] = {
        StaticMeshObjectActor::StaticMeshObjectActorIdentifier,
        SkeletalMeshObjectActor::SkeletalMeshObjectActorIdentifier,
        EnvironmentActor::EnvironmentActorIdentifier,
    };

    static int ItemCurrentSelectedIdx = 0;
    ImGui::Combo("Select Class", &ItemCurrentSelectedIdx, ItemIdentifiers, IM_ARRAYSIZE(ItemIdentifiers));

    Separator();

    EPlaceableObjectKind ObjectKind = (EPlaceableObjectKind)(ItemCurrentSelectedIdx + 1);
    if (Button("OK", ImVec2(120, 0)))
    {
        switch (ObjectKind)
        {
        case EPlaceableObjectKind::STATIC_MESH_ACTOR_KIND:
        {
            CurrentMapAssetCached->PlaceableAddHelper<StaticMeshObjectActor>(nullptr);
            break;
        }
        case EPlaceableObjectKind::SKELETAL_MESH_ACTOR_KIND:
        {
            CurrentMapAssetCached->PlaceableAddHelper<SkeletalMeshObjectActor>(nullptr);
            break;
        }
        case EPlaceableObjectKind::ENVIORNMENT_ACTOR_KIND:
        {
            CurrentMapAssetCached->PlaceableAddHelper<EnvironmentActor>();
            break;
        }
        }
        CloseCurrentPopup();
    }

    SetItemDefaultFocus();
    SameLine();
    if (Button("Cancel", ImVec2(120, 0))) { CloseCurrentPopup(); }
}
