#include "AddAttachableModal.h"

#include "EditorWorld.h"
#include "MapAsset.h"

#include "Camera.h"
#include "SkeletalMeshObject.h"
#include "StaticMeshObject.h"
#include "BoundingSphereObject.h"
#include "OBBObject.h"

#include "GlobalVariable.h"

using namespace std;
using namespace ImGui;

AddAttachableModal::AddAttachableModal(
    const string& ModalHeaderNameIn,
    EditorWorld* EditorWorldIn
)
    : AObjectManageModal(ModalHeaderNameIn, EditorWorldIn)
{
}

AddAttachableModal::~AddAttachableModal()
{
}

bool AddAttachableModal::ModalCondition()
{
    SameLine();
	const char* SmallButtonText = "Add Attachable +";
	float ButtonWidth = CalcTextSize(SmallButtonText).x;
	SetCursorPosX(GetCursorPosX() + GetContentRegionAvail().x - ButtonWidth);
	return SmallButton(SmallButtonText);
}

void AddAttachableModal::RenderModal()
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
                    SelectedAttached->AddAttachedObject<StaticMeshObject>(CurrentMapAssetCached, nullptr) :
                    SelectedPlaced->AddAttachedObject<StaticMeshObject>(CurrentMapAssetCached, nullptr);
                break;
            }
            case EAttachableObjectKind::SKELETAL_MESH_KIND:
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<SkeletalMeshObject>(CurrentMapAssetCached, nullptr) :
                    SelectedPlaced->AddAttachedObject<SkeletalMeshObject>(CurrentMapAssetCached, nullptr);
                break;
            }
            case EAttachableObjectKind::BOUNDING_SPHERE_KIND:
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<BoundingSphereObject>(CurrentMapAssetCached) :
                    SelectedPlaced->AddAttachedObject<BoundingSphereObject>(CurrentMapAssetCached);
                break;
            }
            case EAttachableObjectKind::OBB_KIND:
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<OBBObject>(CurrentMapAssetCached) :
                    SelectedPlaced->AddAttachedObject<OBBObject>(CurrentMapAssetCached);
                break;
            }
            case EAttachableObjectKind::SDR_CAMERA_KIND:
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<Camera>(CurrentMapAssetCached, App::GWidth, App::GHeight) :
                    SelectedPlaced->AddAttachedObject<Camera>(CurrentMapAssetCached, App::GWidth, App::GHeight);
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
