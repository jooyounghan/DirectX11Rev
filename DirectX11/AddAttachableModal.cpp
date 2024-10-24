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

AddAttachableModal::AddAttachableModal(
    const string& ModalHeaderNameIn,
    EditorWorld* EditorWorldIn
)
    : AObjectManageModal(ModalHeaderNameIn, EditorWorldIn)
{
    AttachableItemIdentifiers =
    {
        StaticMeshObject::StaticMeshObjectKind,
        SkeletalMeshObject::SkeletalMeshObjectKind,
        BoundingSphereObject::BoundingSphereKind,
        OBBObject::BoundingOBBKind,
        SDRCamera::SDRCameraKind,
        HDRCamera::HDRCameraKind
    };
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
        static string SelectedAttachableKind;

        if (ImGui::BeginCombo("Select Attachable Object", SelectedAttachableKind.empty() ? "Select Attachable Object" : SelectedAttachableKind.c_str()))
        {
            for (const string& AttachableItemIdentifier : AttachableItemIdentifiers)
            {
                if (ImGui::Selectable(AttachableItemIdentifier.c_str()))
                {
                    SelectedAttachableKind = AttachableItemIdentifier;
                }
            }
            ImGui::EndCombo();
        }

        Separator();

        if (Button("OK", ImVec2(120, 0)))
        {
            if (SelectedAttachableKind == StaticMeshObject::StaticMeshObjectKind)
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<StaticMeshObject>(CurrentMapAssetCached, nullptr) :
                    SelectedPlaced->AddAttachedObject<StaticMeshObject>(CurrentMapAssetCached, nullptr);
            }
            else if (SelectedAttachableKind == SkeletalMeshObject::SkeletalMeshObjectKind)
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<SkeletalMeshObject>(CurrentMapAssetCached, nullptr) :
                    SelectedPlaced->AddAttachedObject<SkeletalMeshObject>(CurrentMapAssetCached, nullptr);
            }                              
            else if (SelectedAttachableKind == BoundingSphereObject::BoundingSphereKind)
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<BoundingSphereObject>(CurrentMapAssetCached) :
                    SelectedPlaced->AddAttachedObject<BoundingSphereObject>(CurrentMapAssetCached);
            }
            else if (SelectedAttachableKind == OBBObject::BoundingOBBKind)
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<OBBObject>(CurrentMapAssetCached) :
                    SelectedPlaced->AddAttachedObject<OBBObject>(CurrentMapAssetCached);
            }
            else if (SelectedAttachableKind == SDRCamera::SDRCameraKind)
            {
                SelectedAttached != nullptr ?
                    SelectedAttached->AddAttachedObject<SDRCamera>(CurrentMapAssetCached, App::GWidth, App::GHeight) :
                    SelectedPlaced->AddAttachedObject<SDRCamera>(CurrentMapAssetCached, App::GWidth, App::GHeight);
            }
            else if (SelectedAttachableKind == HDRCamera::HDRCameraKind)
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
