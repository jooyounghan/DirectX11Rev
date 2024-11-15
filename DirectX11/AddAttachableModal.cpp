#include "AddAttachableModal.h"

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

AddAttachableModal::AddAttachableModal(const string& ModalHeaderNameIn)
    : AObjectManageModal(ModalHeaderNameIn)
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
	return SmallButton(SmallButtonText) && (CurrentMapAssetCached != nullptr);
}

void AddAttachableModal::RenderModal()
{
    AAttachableObject* SelectedAttachable = CurrentMapAssetCached->GetSelectedAttachable();
    APlaceableObject* SelectedPlaced = CurrentMapAssetCached->GetSelectedPlaceable();

    if (SelectedPlaced != nullptr || SelectedAttachable != nullptr)
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
            MapAsset* CurrentMap = CurrentMapAssetCached.get();
            if (SelectedAttachableKind == StaticMeshObject::StaticMeshObjectKind)
            {
                SelectedAttachable != nullptr ?
                    SelectedAttachable->AddAttachedObject<StaticMeshObject>(CurrentMap, nullptr) :
                    SelectedPlaced->AddAttachedObject<StaticMeshObject>(CurrentMap, nullptr);
            }
            else if (SelectedAttachableKind == SkeletalMeshObject::SkeletalMeshObjectKind)
            {
                SelectedAttachable != nullptr ?
                    SelectedAttachable->AddAttachedObject<SkeletalMeshObject>(CurrentMap, nullptr) :
                    SelectedPlaced->AddAttachedObject<SkeletalMeshObject>(CurrentMap, nullptr);
            }                              
            else if (SelectedAttachableKind == BoundingSphereObject::BoundingSphereKind)
            {
                SelectedAttachable != nullptr ?
                    SelectedAttachable->AddAttachedObject<BoundingSphereObject>(CurrentMap) :
                    SelectedPlaced->AddAttachedObject<BoundingSphereObject>(CurrentMap);
            }
            else if (SelectedAttachableKind == OBBObject::BoundingOBBKind)
            {
                SelectedAttachable != nullptr ?
                    SelectedAttachable->AddAttachedObject<OBBObject>(CurrentMap) :
                    SelectedPlaced->AddAttachedObject<OBBObject>(CurrentMap);
            }
            else if (SelectedAttachableKind == SDRCamera::SDRCameraKind)
            {
                SelectedAttachable != nullptr ?
                    SelectedAttachable->AddAttachedObject<SDRCamera>(CurrentMap, App::GWidth, App::GHeight) :
                    SelectedPlaced->AddAttachedObject<SDRCamera>(CurrentMap, App::GWidth, App::GHeight);
            }
            else if (SelectedAttachableKind == HDRCamera::HDRCameraKind)
            {
                SelectedAttachable != nullptr ?
                    SelectedAttachable->AddAttachedObject<HDRCamera>(CurrentMap, App::GWidth, App::GHeight) :
                    SelectedPlaced->AddAttachedObject<HDRCamera>(CurrentMap, App::GWidth, App::GHeight);
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
