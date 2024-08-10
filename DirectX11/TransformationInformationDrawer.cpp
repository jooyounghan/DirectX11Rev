#include "TransformationInformationDrawer.h"
#include "APlaceable.h"

#include "DefineType.h"

#include <string>
#include <format>

using namespace std;
using namespace ImGui;

const char* TransformationInformationDrawer::TransformationSelect[NumTransformationSelect]
{
    "Absolute", "Relative" 
};

TransformationInformationDrawer::TransformationInformationDrawer(
    APlaceable* CurrentPlaceable, APlaceable* ParentPlaceable,
    const bool& IsTranslationOnlyAbsoluteIn, const bool& IsTranslationDisabledIn,
    const bool& IsRotationOnlyAbsoluteIn, const bool& IsRotationDisabledIn,
    const bool& IsScalingOnlyAbsoluteIn, const bool& IsScalingDisabledIn
)
    : AInformationDrawer(CurrentPlaceable), ParentObjectCached(ParentPlaceable),
    IsTranslationOnlyAbsolute(IsTranslationOnlyAbsoluteIn),
    IsTranslationDisabled(IsTranslationDisabledIn),
    IsRotationOnlyAbsolute(IsRotationOnlyAbsoluteIn),
    IsRotationDisabled(IsRotationDisabledIn),
    IsScalingOnlyAbsolute(IsScalingOnlyAbsoluteIn),
    IsScalingDisabled(IsScalingDisabledIn)
{
}

void TransformationInformationDrawer::DrawInformation()
{
    static ETransfomationSelect SelectedTranslationIndex = Absolute;
    static ETransfomationSelect SelectedRotationIndex = Absolute;
    static ETransfomationSelect SelectedScaleIndex = Absolute;

    SPosition4D DummyPosition;
    SAngle DummyAngle;
    SVector3D DummyScale;

    ImGui::SeparatorText("Transformation");

    DrawTransformationEntitySelection(IsTranslationOnlyAbsolute, "Translation", SelectedTranslationIndex);
    SameLine();
    DrawTransformationEntity<SPosition4D>(
        SelectedTranslationIndex, 
        ObjectCached ? ObjectCached->Position : DummyPosition,
        ParentObjectCached ? ParentObjectCached->Position : SPosition4D(),
        IsTranslationDisabled
    );

    DrawTransformationEntitySelection(IsRotationOnlyAbsolute, "Rotation", SelectedRotationIndex);
    SameLine();
    DrawTransformationEntity<SAngle>(
        SelectedRotationIndex, 
        ObjectCached ? ObjectCached->Angle : DummyAngle,
        ParentObjectCached ? ParentObjectCached->Angle : SAngle(),
        IsRotationDisabled);

    DrawTransformationEntitySelection(IsScalingOnlyAbsolute, "Scale", SelectedScaleIndex);
    SameLine();
    DrawTransformationEntity<SVector3D>(
        SelectedScaleIndex, 
        ObjectCached ? ObjectCached->Scale : DummyScale,
        ParentObjectCached ? ParentObjectCached->Scale : SVector3D(),
        IsScalingDisabled
    );
}


void TransformationInformationDrawer::DrawTransformationEntitySelection(
    const bool& IsOnlyAbsoluteIn, 
    const char* EntityName, 
    ETransfomationSelect& SelectedIndexOut
)
{
    const char* PreviewText = TransformationSelect[SelectedIndexOut];
    if (ImGui::BeginCombo(EntityName, PreviewText, ImGuiComboFlags_WidthFitPreview))
    {
        for (size_t idx = 0; idx < (IsOnlyAbsoluteIn ? (size_t)Relative : (size_t)NumTransformationSelect); ++idx)
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
inline void TransformationInformationDrawer::DrawTransformationEntity(
    const ETransfomationSelect& SelectedIndex,
    T& Entity,
    const T& ParentEntity,
    const bool& DisabledIn
)
{
    const std::string EntityId{ std::format("{}", (uint64_t)&Entity) };
    ImGui::PushID(EntityId.c_str());
    switch (SelectedIndex)
    {
    case ETransfomationSelect::Absolute:
    {
        T TempEntity = Entity + ParentEntity;
        if (DisabledIn)
        {
            ImGui::BeginDisabled();
            ImGui::DragFloat3("", (float*)&TempEntity);
            ImGui::EndDisabled();
        }
        else
        {
            ImGui::DragFloat3("", (float*)&TempEntity);
        }
        Entity = TempEntity - ParentEntity;
        break;
    }
    case ETransfomationSelect::Relative:
    {
        if (DisabledIn)
        {
            ImGui::BeginDisabled();
            ImGui::DragFloat3("", (float*)&Entity);
            ImGui::EndDisabled();
        }
        else
        {
            ImGui::DragFloat3("", (float*)&Entity);
        }
        break;
    }
    }
    ImGui::PopID();
}
