#include "AddAttachableObjectHelper.h"

#include "EditorWorld.h"
#include "GlobalVariable.h"

// Add Attachables
#include "PlaceableObject.h"
#include "MeshObject.h"
#include "BoundingSphereObject.h"
#include "OBBObject.h"
#include "Camera.h"

#include <list>
#include <memory>

using namespace std;
using namespace ImGui;

AddAttachableObjectDrawer::AddAttachableObjectDrawer(
    EditorWorld* EditorWorldIn, 
    MapAsset* CurrentMap
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
            BoundingSphereObject::BoundingSphereIdentifier,
            OBBObject::BoundingOBBIdentifier,
            Camera::CameraIdentifier
        };

        static int ItemCurrentSelectedIdx = 0;
        ImGui::Combo("Select Class", &ItemCurrentSelectedIdx, ItemIdentifiers, IM_ARRAYSIZE(ItemIdentifiers));

        Separator();

        GraphicsPipeline* GraphicPipelineCached = EditorWorldCached->GraphicsPipelineCached;


        EAttachableObjectKind ObjectKind = (EAttachableObjectKind)(ItemCurrentSelectedIdx + 1);
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
                        SelectedAttached->AddAttachedObject<BoundingSphereObject>(GraphicPipelineCached) :
                        SelectedPlaced->AddAttachedObject<BoundingSphereObject>(GraphicPipelineCached);
                    break;
                }
                case EAttachableObjectKind::OrientedBoundingBoxKind:
                {
                    SelectedAttached != nullptr ?
                        SelectedAttached->AddAttachedObject<OBBObject>(GraphicPipelineCached) :
                        SelectedPlaced->AddAttachedObject<OBBObject>(GraphicPipelineCached);
                    break;
                }
                case EAttachableObjectKind::NormalCameraKind:
                {
                    SelectedAttached != nullptr ?
                        SelectedAttached->AddAttachedObject<Camera>(GraphicPipelineCached, App::GWidth, App::GHeight) :
                        SelectedPlaced->AddAttachedObject<Camera>(GraphicPipelineCached, App::GWidth, App::GHeight);
                    break;
                }
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
