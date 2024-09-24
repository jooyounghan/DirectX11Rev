#include "AddPlaceableObjectHelper.h"

#include "EditorWorld.h"
#include "MapAsset.h"

// Add Placeables
#include "EnvironmentActor.h"
#include "StaticMeshObjectActor.h"
#include "SkeletalMeshObjectActor.h"

using namespace ImGui;

AddPlaceableObjectDrawer::AddPlaceableObjectDrawer(
    EditorWorld* EditorWorldIn, 
    MapAsset* CurrentMap
)
    : AAddObjectHelper(EditorWorldIn, CurrentMap)
{
}

void AddPlaceableObjectDrawer::DrawAddObjectPopup()
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
            CurrentMapCached->PlaceableAddHelper<StaticMeshObjectActor>(nullptr);
            break;
        }
        case EPlaceableObjectKind::SKELETAL_MESH_ACTOR_KIND:
        {
            CurrentMapCached->PlaceableAddHelper<SkeletalMeshObjectActor>(nullptr);
            break;
        }
        case EPlaceableObjectKind::ENVIORNMENT_ACTOR_KIND:
        {
            CurrentMapCached->PlaceableAddHelper<EnvironmentActor>();
            break;
        }
        }
        CloseCurrentPopup();
    }

    SetItemDefaultFocus();
    SameLine();
    if (Button("Cancel", ImVec2(120, 0))) { CloseCurrentPopup(); }
}
