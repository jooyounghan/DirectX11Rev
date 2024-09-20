#include "AddAttachableObjectHelper.h"

#include "EditorWorld.h"
#include "GlobalVariable.h"

// Add Attachables
#include "APlaceableObject.h"

#include "StaticMeshObject.h"
#include "SkeletalMeshObject.h"
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
    AAttachableObject* SelectedAttached = EditorWorldCached->GetSelectedAttached();
    APlaceableObject* SelectedPlaced = EditorWorldCached->GetSelectedPlaceable();

    if (SelectedPlaced != nullptr || SelectedAttached != nullptr)
    {
        const char* ItemIdentifiers[] = {
            StaticMeshObject::StaticMeshObjectIdentifier,
            SkeletalMeshObject::SkeletalMeshObjectIdentifier,
            BoundingSphereObject::BoundingSphereIdentifier,
            OBBObject::BoundingOBBIdentifier,
            Camera::CameraIdentifier
        };

        static int ItemCurrentSelectedIdx = 0;
        ImGui::Combo("Select Class", &ItemCurrentSelectedIdx, ItemIdentifiers, IM_ARRAYSIZE(ItemIdentifiers));

        Separator();

        EAttachableObjectKind ObjectKind = (EAttachableObjectKind)(ItemCurrentSelectedIdx + 1);
        if (Button("OK", ImVec2(120, 0))) 
        { 
            switch (ObjectKind)
            {
                case EAttachableObjectKind::STATIC_MESH_KIND:
                {
                    SelectedAttached != nullptr ?
                        SelectedAttached->AddAttachedObject<StaticMeshObject>(nullptr) :
                        SelectedPlaced->AddAttachedObject<StaticMeshObject>(nullptr);
                    break;
                }
                case EAttachableObjectKind::SKELETAL_MESH_KIND:
                {
                    SelectedAttached != nullptr ?
                        SelectedAttached->AddAttachedObject<SkeletalMeshObject>(nullptr) :
                        SelectedPlaced->AddAttachedObject<SkeletalMeshObject>(nullptr);
                    break;
                }
                case EAttachableObjectKind::BOUNDING_SPHERE_KIND:
                {
                    SelectedAttached != nullptr ?
                        SelectedAttached->AddAttachedObject<BoundingSphereObject>() :
                        SelectedPlaced->AddAttachedObject<BoundingSphereObject>();
                    break;
                }
                case EAttachableObjectKind::OBB_KIND:
                {
                    SelectedAttached != nullptr ?
                        SelectedAttached->AddAttachedObject<OBBObject>() :
                        SelectedPlaced->AddAttachedObject<OBBObject>();
                    break;
                }
                case EAttachableObjectKind::SDR_CAMERA_KIND:
                {
                    SelectedAttached != nullptr ?
                        SelectedAttached->AddAttachedObject<Camera>(App::GWidth, App::GHeight) :
                        SelectedPlaced->AddAttachedObject<Camera>(App::GWidth, App::GHeight);
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
