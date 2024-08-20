#include "AddAttachableObjectHelper.h"

#include "EditorWorld.h"


#include "PlaceableObject.h"
#include "MeshObject.h"
#include "BoundingSphere.h"
#include "OrientedBoundingBox.h"

#include <list>
#include <memory>

using namespace std;
using namespace ImGui;

AddAttachableObjectDrawer::AddAttachableObjectDrawer(
    EditorWorld* EditorWorldIn, 
    Map* CurrentMap
)
    : AAddObjectHelper(EditorWorldIn, CurrentMap)
{
}

void AddAttachableObjectDrawer::DrawAddObjectPopup()
{
    AttachableObject* SelectedAttached = EditorWorldCached->GetSelectedAttached();
    PlaceableObject* SelectedPlaced = EditorWorldCached->GetSelectedPlaceable();

    if (SelectedPlaced != nullptr || SelectedAttached != nullptr)
    {
        const char* ItemIdentifiers[] = {
            MeshObject::MeshObjectIdentifier,
            BoundingSphere::BoundingSphereIdentifier,
            OrientedBoundingBox::BoundingOBBIdentifier
        };

        static int ItemCurrentSelectedIdx = 0;
        ImGui::Combo("Select Class", &ItemCurrentSelectedIdx, ItemIdentifiers, IM_ARRAYSIZE(ItemIdentifiers));

        Separator();

        GraphicsPipeline* GraphicPipelineCached = EditorWorldCached->GraphicsPipelineCached;


        EAttachableObjectKind ObjectKind = (EAttachableObjectKind)ItemCurrentSelectedIdx;
        if (Button("OK", ImVec2(120, 0))) 
        { 
            switch (ObjectKind)
            {
            case EAttachableObjectKind::MeshObjectKind:
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<MeshObject>(GraphicPipelineCached) :
                    SelectedPlaced->AddAttachedObject<MeshObject>(GraphicPipelineCached);
                break;
            }
            case EAttachableObjectKind::BoundingSphereKind:
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<BoundingSphere>(GraphicPipelineCached) :
                    SelectedPlaced->AddAttachedObject<BoundingSphere>(GraphicPipelineCached);
                break;
            }
            case EAttachableObjectKind::OrientedBoundingBoxKind:
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<OrientedBoundingBox>(GraphicPipelineCached) :
                    SelectedPlaced->AddAttachedObject<OrientedBoundingBox>(GraphicPipelineCached);
                break;
            }
            default:
                break;
            }
            CloseCurrentPopup(); 
        }

        SetItemDefaultFocus();
        SameLine();
        if (Button("Cancel", ImVec2(120, 0))) { CloseCurrentPopup(); }
    }
    else
    {
        CloseCurrentPopup();
    }
}
