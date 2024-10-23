#include "AddAttachableModal.h"

#include "EditorWorld.h"
#include "MapAsset.h"

#include "SkeletalMeshObject.h"
#include "StaticMeshObject.h"
#include "BoundingSphereObject.h"
#include "OBBObject.h"

#include "SDRCamera.h"
#include "HDRCamera.h"

#include "GlobalVariable.h"

using namespace std;
using namespace ImGui;

const char* AddAttachableModal::AttachableItemIdentifiers[] = 
{
    StaticMeshObject::StaticMeshObjectKind.c_str(),
    SkeletalMeshObject::SkeletalMeshObjectKind.c_str(),
    BoundingSphereObject::BoundingSphereKind.c_str(),
    OBBObject::BoundingOBBKind.c_str(),
    SDRCamera::SDRCameraKind.c_str(),
    HDRCamera::HDRCameraKind.c_str()
};


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
        static int ItemCurrentSelectedIdx = 0;
        ImGui::Combo("Select Class", &ItemCurrentSelectedIdx, AttachableItemIdentifiers, IM_ARRAYSIZE(AttachableItemIdentifiers));

        Separator();

        const char* SelectedObjectKind = AttachableItemIdentifiers[ItemCurrentSelectedIdx];

        if (Button("OK", ImVec2(120, 0)))
        {
            if (SelectedObjectKind == StaticMeshObject::StaticMeshObjectKind)
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<StaticMeshObject>(CurrentMapAssetCached, nullptr) :
                    SelectedPlaced->AddAttachedObject<StaticMeshObject>(CurrentMapAssetCached, nullptr);
            }
            else if (SelectedObjectKind == SkeletalMeshObject::SkeletalMeshObjectKind)
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<SkeletalMeshObject>(CurrentMapAssetCached, nullptr) :
                    SelectedPlaced->AddAttachedObject<SkeletalMeshObject>(CurrentMapAssetCached, nullptr);
            }                              
            else if (SelectedObjectKind == BoundingSphereObject::BoundingSphereKind)
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<BoundingSphereObject>(CurrentMapAssetCached) :
                    SelectedPlaced->AddAttachedObject<BoundingSphereObject>(CurrentMapAssetCached);
            }
            else if (SelectedObjectKind == OBBObject::BoundingOBBKind)
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<OBBObject>(CurrentMapAssetCached) :
                    SelectedPlaced->AddAttachedObject<OBBObject>(CurrentMapAssetCached);
            }
            else if (SelectedObjectKind == SDRCamera::SDRCameraKind)
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<SDRCamera>(CurrentMapAssetCached, App::GWidth, App::GHeight) :
                    SelectedPlaced->AddAttachedObject<SDRCamera>(CurrentMapAssetCached, App::GWidth, App::GHeight);
            }
            else if (SelectedObjectKind == HDRCamera::HDRCameraKind)
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<HDRCamera>(CurrentMapAssetCached, App::GWidth, App::GHeight) :
                    SelectedPlaced->AddAttachedObject<HDRCamera>(CurrentMapAssetCached, App::GWidth, App::GHeight);
            }
            else
            {

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
