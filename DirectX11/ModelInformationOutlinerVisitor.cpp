#include "ModelInformationOutlinerVisitor.h"
#include "MapOutlinerWindow.h"

#include "MeshObject.h"

#include "BoundingSphere.h"
#include "OrientedBoundingBox.h"
#include "BoundingFrustum.h"

#include "Viewable.h"

#include <format>



// 특정 개체 참조 후 삭제 예정
#include "AttachableObject.h"
#include "PlaceableObject.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;

const char* ModelInformationOutlinerVisitor::TransformationSelect[NumTransformationSelect] { "Absolute", "Relative" };

ModelInformationOutlinerVisitor::ModelInformationOutlinerVisitor(MapOutlinerWindow* MapOutlinerInstance)
	: MapOutlinerCached(MapOutlinerInstance)
{
}

void ModelInformationOutlinerVisitor::Visit(MeshObject* AttachableInstance)
{
    DrawRelativePlaceableInformation(AttachableInstance);
}

void ModelInformationOutlinerVisitor::Visit(BoundingSphere* BoundingSphereInstance)
{
    const string ComboId{ std::format("{}", (uint64_t)BoundingSphereInstance) };
    PushID(ComboId.c_str());

    DrawRelativePlaceableInformation(BoundingSphereInstance);

    ImGui::SeparatorText("Boundging Sphere");
    DragFloat("Radius", BoundingSphereInstance->GetPointerRadius(), 1.f, static_cast<float>(1E-3), numeric_limits<float>::max());

    PopID();
}

void ModelInformationOutlinerVisitor::Visit(OrientedBoundingBox* OBBInstance)
{
    const string ComboId{ std::format("{}", (uint64_t)OBBInstance) };
    PushID(ComboId.c_str());

    DrawRelativePlaceableInformation(OBBInstance);
    
    ImGui::SeparatorText("Oriented Bounding Box");
    DragFloat3("Half Extends", OBBInstance->GetPointerHalfExtends(), 1.f, static_cast<float>(1E-3), numeric_limits<float>::max());

    PopID();
}

void ModelInformationOutlinerVisitor::Visit(BoundingFrustum* BoundingFrustumInstance)
{
    const string ComboId{ std::format("{}", (uint64_t)BoundingFrustumInstance) };
    PushID(ComboId.c_str());

    DrawRelativePlaceableInformation(BoundingFrustumInstance);

    PopID();
}

void ModelInformationOutlinerVisitor::Visit(Viewable* ViewableInstance)
{
    const string ComboId{ std::format("{}", (uint64_t)ViewableInstance) };
    PushID(ComboId.c_str());

    DrawRelativePlaceableInformation(ViewableInstance);
   
    PopID();
}



void ModelInformationOutlinerVisitor::DrawAttachableInformation(AttachableObject* Attachable)
{
    DrawTransformationForAttachable(Attachable);
}

void ModelInformationOutlinerVisitor::DrawPlaceableInformation(PlaceableObject* Placeable)
{
    DrawTransformationForPlaceables(Placeable);
}

void ModelInformationOutlinerVisitor::DrawRelativePlaceableInformation(RelativePlaceableObject* RelativePlaceableObject)
{
    DrawTransformationForRelativePlaceable(RelativePlaceableObject);
}

void ModelInformationOutlinerVisitor::DrawTransformationForAttachable(AttachableObject* Attachable)
{
    static ETransfomationSelect SelectedTranslationIndex = Absolute;
    static ETransfomationSelect SelectedRotationIndex = Absolute;
    static ETransfomationSelect SelectedScaleIndex = Absolute;

    APlaceable* ParentObject = Attachable->GetParentObject();

    static SPosition4D DummyPosition;
    static SAngle DummyAngle;
    static SVector3D DummyScale;

    ImGui::SeparatorText("Transformation");

    DrawTransformationEntitySelection(true, "Translation", SelectedTranslationIndex);
    SameLine();
    DrawTransformationEntity<SPosition4D>(SelectedTranslationIndex, DummyPosition, ParentObject != nullptr ? ParentObject->Position : SPosition4D(), true);

    DrawTransformationEntitySelection(true, "Rotation", SelectedRotationIndex);
    SameLine();
    DrawTransformationEntity<SAngle>(SelectedRotationIndex, DummyAngle, ParentObject != nullptr ? ParentObject->Angle : SAngle(), true);

    DrawTransformationEntitySelection(true, "Scale", SelectedScaleIndex);
    SameLine();
    DrawTransformationEntity<SVector3D>(SelectedScaleIndex, DummyScale, SVector3D(), true);
}

void ModelInformationOutlinerVisitor::DrawTransformationForPlaceables(PlaceableObject* Placeable)
{
    static ETransfomationSelect SelectedTranslationIndex = Absolute;
    static ETransfomationSelect SelectedRotationIndex = Absolute;
    static ETransfomationSelect SelectedScaleIndex = Absolute;

    ImGui::SeparatorText("Transformation");

    DrawTransformationEntitySelection(true, "Translation", SelectedTranslationIndex);
    SameLine();
    DrawTransformationEntity<SPosition4D>(SelectedTranslationIndex, Placeable->Position, SPosition4D(), false);

    DrawTransformationEntitySelection(true, "Rotation", SelectedRotationIndex);
    SameLine();
    DrawTransformationEntity<SAngle>(SelectedRotationIndex, Placeable->Angle, SAngle(), false);

    DrawTransformationEntitySelection(true, "Scale", SelectedScaleIndex);
    SameLine();
    DrawTransformationEntity<SVector3D>(SelectedScaleIndex, Placeable->Scale, SVector3D(), false);
}

void ModelInformationOutlinerVisitor::DrawTransformationForRelativePlaceable(RelativePlaceableObject* RelativePlaceableObject)
{
    static ETransfomationSelect SelectedTranslationIndex = Absolute;
    static ETransfomationSelect SelectedRotationIndex = Absolute;
    static ETransfomationSelect SelectedScaleIndex = Absolute;

    APlaceable* ParentObject = RelativePlaceableObject->GetParentObject();

    ImGui::SeparatorText("Transformation");

    DrawTransformationEntitySelection(false, "Translation", SelectedTranslationIndex);
    SameLine();
    DrawTransformationEntity<SPosition4D>(SelectedTranslationIndex, RelativePlaceableObject->Position, ParentObject != nullptr ? ParentObject->Position : SPosition4D(), false);

    DrawTransformationEntitySelection(false, "Rotation", SelectedRotationIndex);
    SameLine();
    DrawTransformationEntity<SAngle>(SelectedRotationIndex, RelativePlaceableObject->Angle, ParentObject != nullptr ? ParentObject->Angle : SAngle(), false);

    DrawTransformationEntitySelection(true, "Scale", SelectedScaleIndex);
    SameLine();
    DrawTransformationEntity<SVector3D>(SelectedScaleIndex, RelativePlaceableObject->Scale, SVector3D(), false);
}

void ModelInformationOutlinerVisitor::DrawTransformationEntitySelection(const bool& IsOnlyAbsolute, const char* EntityName, ETransfomationSelect& SelectedIndexOut)
{
    const char* PreviewText = TransformationSelect[SelectedIndexOut];
    if (ImGui::BeginCombo(EntityName, PreviewText, ImGuiComboFlags_WidthFitPreview))
    {
        for (size_t idx = 0; idx < (IsOnlyAbsolute ? (size_t)Relative : (size_t)NumTransformationSelect); ++idx)
        {
            const ETransfomationSelect CurrentSelectIdx = static_cast<ETransfomationSelect>(idx);
            const bool IsSelected = (SelectedIndexOut == CurrentSelectIdx);
            if (ImGui::Selectable(TransformationSelect[idx], IsSelected, NULL))
                SelectedIndexOut = CurrentSelectIdx;

            if (IsSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

template<typename T>
void ModelInformationOutlinerVisitor::DrawTransformationEntity(
    const ETransfomationSelect& SelectedIndex, 
    T& Entity, 
    const T& ParentEntity,
    const bool& Disabled
)
{
    const string EntityId{ std::format("{}", (uint64_t)&Entity) };
    PushID(EntityId.c_str());
    switch (SelectedIndex)
    {
    case ETransfomationSelect::Absolute:
    {
        T TempEntity = Entity + ParentEntity;
        if (Disabled)
        {
            BeginDisabled();
            DragFloat3("", (float*)&TempEntity);
            EndDisabled();
        }
        else
        {
            DragFloat3("", (float*)&TempEntity);
        }
        Entity = TempEntity - ParentEntity;
        break;
    }
    case ETransfomationSelect::Relative:
    {
        if (Disabled)
        {
            BeginDisabled();
            DragFloat3("", (float*)&Entity);
            EndDisabled();
        }
        else
        {
            DragFloat3("", (float*)&Entity);
        }
        break;
    }
    }
    PopID();
}

