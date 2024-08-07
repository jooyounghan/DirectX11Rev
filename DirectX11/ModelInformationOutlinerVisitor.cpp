#include "ModelInformationOutlinerVisitor.h"
#include "MapOutlinerWindow.h"

#include "AttachableObject.h"
#include "PlaceableObject.h"
#include "RelativePlaceableObject.h"

#include <format>

using namespace std;
using namespace ImGui;
using namespace DirectX;

const char* ModelInformationOutlinerVisitor::TransformationSelect[NumTransformationSelect] { "Absolute", "Relative" };

ModelInformationOutlinerVisitor::ModelInformationOutlinerVisitor(MapOutlinerWindow* MapOutlinerInstance)
	: MapOutlinerCached(MapOutlinerInstance)
{
}

void ModelInformationOutlinerVisitor::Visit(AttachableObject* AttachableInstance)
{
	return DrawAttachableInformation(AttachableInstance);
}

void ModelInformationOutlinerVisitor::Visit(PlaceableObject* PlaceableInstance)
{
	return DrawPlaceableInformation(PlaceableInstance);
}

void ModelInformationOutlinerVisitor::Visit(RelativePlaceableObject* RelativePlaceableObject)
{
	return DrawRelativePlaceableInformation(RelativePlaceableObject);
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
}

void ModelInformationOutlinerVisitor::DrawTransformationForPlaceables(PlaceableObject* Placeable)
{
}

void ModelInformationOutlinerVisitor::DrawTransformationForRelativePlaceable(RelativePlaceableObject* RelativePlaceableObject)
{
    static ETransfomationSelect SelectedTranslationIndex = Absolute;
    static ETransfomationSelect SelectedRotationIndex = Absolute;
    static ETransfomationSelect SelectedScaleIndex = Absolute;

    APlaceable* ParentObject = RelativePlaceableObject->GetParentObject();

    ImGui::SeparatorText("Transformation");

    const string ComboId{ std::format("{}", (uint64_t)RelativePlaceableObject) };
    PushID(ComboId.c_str());

    DrawTransformationEntitySelection(false, "Translation", SelectedTranslationIndex);
    SameLine();
    DrawTransformationEntity<SPosition4D>(SelectedTranslationIndex, RelativePlaceableObject->Position, ParentObject != nullptr ? ParentObject->Position : SPosition4D());

    DrawTransformationEntitySelection(false, "Rotation", SelectedRotationIndex);
    SameLine();
    DrawTransformationEntity<SAngle>(SelectedRotationIndex, RelativePlaceableObject->Angle, ParentObject != nullptr ? ParentObject->Angle : SAngle());

    DrawTransformationEntitySelection(false, "Scale", SelectedScaleIndex);
    SameLine();
    DrawTransformationEntity<SVector3D>(SelectedScaleIndex, RelativePlaceableObject->Scale, ParentObject != nullptr ? ParentObject->Scale : SVector3D());

    PopID();
}

void ModelInformationOutlinerVisitor::DrawTransformationEntitySelection(const bool& IsOnlyAttached, const char* EntityName, ETransfomationSelect& SelectedIndexOut)
{
    const char* PreviewText = TransformationSelect[SelectedIndexOut];
    if (ImGui::BeginCombo(EntityName, PreviewText, ImGuiComboFlags_WidthFitPreview))
    {
        for (size_t idx = 0; idx < (IsOnlyAttached ? (size_t)Relative : (size_t)NumTransformationSelect); ++idx)
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
    const T& ParentEntity
)
{
    const string EntityId{ std::format("{}", (uint64_t)&Entity) };
    PushID(EntityId.c_str());
    switch (SelectedIndex)
    {
    case ETransfomationSelect::Absolute:
    {
        T TempEntity = Entity + ParentEntity;
        DragFloat3("", (float*)&TempEntity);
        Entity = TempEntity - ParentEntity;
        break;
    }
    case ETransfomationSelect::Relative:
    {
        DragFloat3("", (float*)&Entity);
        break;
    }
    }
    PopID();
}

