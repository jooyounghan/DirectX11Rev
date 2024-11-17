#include "AddAttachableModal.h"
#include "MapOutlinerWindow.h"

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

AddAttachableModal::AddAttachableModal(MapOutlinerWindow* MapOutlinerWindowIn, const string& ModalHeaderNameIn)
    : AModal(ModalHeaderNameIn), MapOutlinerWindowCached(MapOutlinerWindowIn)
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
    AAttachableObject* SelectedAttachable = MapOutlinerWindowCached->GetSelectedAttachable();
    APlaceableObject* SelectedPlaced = MapOutlinerWindowCached->GetSelectedPlaceable();

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
            if (SelectedAttachableKind == StaticMeshObject::StaticMeshObjectKind)
            {
                SelectedAttachable != nullptr ?
                    SelectedAttachable->AddAttachedObject<StaticMeshObject>(nullptr) :
                    SelectedPlaced->AddAttachedObject<StaticMeshObject>(nullptr);
            }
            else if (SelectedAttachableKind == SkeletalMeshObject::SkeletalMeshObjectKind)
            {
                SelectedAttachable != nullptr ?
                    SelectedAttachable->AddAttachedObject<SkeletalMeshObject>(nullptr) :
                    SelectedPlaced->AddAttachedObject<SkeletalMeshObject>(nullptr);
            }                              
            else if (SelectedAttachableKind == BoundingSphereObject::BoundingSphereKind)
            {
                SelectedAttachable != nullptr ?
                    SelectedAttachable->AddAttachedObject<BoundingSphereObject>() :
                    SelectedPlaced->AddAttachedObject<BoundingSphereObject>();
            }
            else if (SelectedAttachableKind == OBBObject::BoundingOBBKind)
            {
                SelectedAttachable != nullptr ?
                    SelectedAttachable->AddAttachedObject<OBBObject>() :
                    SelectedPlaced->AddAttachedObject<OBBObject>();
            }
            else if (SelectedAttachableKind == SDRCamera::SDRCameraKind)
            {
                SelectedAttachable != nullptr ?
                    SelectedAttachable->AddAttachedObject<SDRCamera>(App::GWidth, App::GHeight) :
                    SelectedPlaced->AddAttachedObject<SDRCamera>(App::GWidth, App::GHeight);
            }
            else if (SelectedAttachableKind == HDRCamera::HDRCameraKind)
            {
                SelectedAttachable != nullptr ?
                    SelectedAttachable->AddAttachedObject<HDRCamera>(App::GWidth, App::GHeight) :
                    SelectedPlaced->AddAttachedObject<HDRCamera>(App::GWidth, App::GHeight);
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
