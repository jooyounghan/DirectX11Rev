#include "TransformationInformationDrawer.h"
#include "AObject.h"

#include "DefineType.h"

#include <string>
#include <format>
#include <functional>

using namespace std;
using namespace ImGui;

const char* TransformationInformationDrawer::TransformationSelect[NumTransformationSelect]
{
    "Absolute", "Relative" 
};

TransformationInformationDrawer::TransformationInformationDrawer(AObject* CurrentPlaceable, AObject* ParentPlaceable, const bool& IsPlaceable)
    : AInformationDrawer(CurrentPlaceable), ParentObjectCached(ParentPlaceable)
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
    AutoZeroMemory(DummyPosition);
    AutoZeroMemory(DummyAngle);
    AutoZeroMemory(DummyScale);

    ImGui::SeparatorText("Transformation");

    DrawEntitySelection("Translation", SelectedTranslationIndex);
    SameLine();
    DrawEntity<SPosition4D, plus<SPosition4D>, minus<SPosition4D>>(
        SelectedTranslationIndex, 
        ObjectCached ? ObjectCached->Position : DummyPosition,
        ParentObjectCached ? ParentObjectCached->Position : SPosition4D(),
        0.1f
    );

    DrawEntitySelection("Rotation", SelectedRotationIndex);
    SameLine();
    DrawEntity<SAngle, plus<SAngle>, minus<SAngle>>(
        SelectedRotationIndex, 
        ObjectCached ? ObjectCached->Angle : DummyAngle,
        ParentObjectCached ? ParentObjectCached->Angle : SAngle(),
        0.1f
);

    DrawEntitySelection("Scale", SelectedScaleIndex);
    SameLine();
    DrawEntity<SVector3D, multiplies<SVector3D>, divides<SVector3D>>(
        SelectedScaleIndex, 
        ObjectCached ? ObjectCached->Scale : DummyScale,
        ParentObjectCached ? ParentObjectCached->Scale : SVector3D(),
        0.01f
    );
}


void TransformationInformationDrawer::DrawEntitySelection(
    const char* EntityName, 
    ETransfomationSelect& SelectedIndexOut
)
{
    const char* PreviewText = TransformationSelect[SelectedIndexOut];
    if (ImGui::BeginCombo(EntityName, PreviewText, ImGuiComboFlags_WidthFitPreview))
    {
        for (size_t idx = 0; idx < (size_t)NumTransformationSelect; ++idx)
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


template<typename T, typename Operator, typename ROperator>
inline void TransformationInformationDrawer::DrawEntity(
    const ETransfomationSelect& SelectedIndex,
    T& Entity,
    const T& ParentEntity,
    const float& SpeedIn
)
{
    Operator Oper;
    ROperator ROper;

    const std::string EntityId{ std::format("{}", (uint64_t)&Entity) };
    ImGui::PushID(EntityId.c_str());
    switch (SelectedIndex)
    {
    case ETransfomationSelect::Absolute:
    {
        T TempEntity = Oper(Entity, ParentEntity);
        ImGui::DragFloat3("", (float*)&TempEntity, SpeedIn);
        Entity = ROper(TempEntity, ParentEntity);
        break;
    }
    case ETransfomationSelect::Relative:
    {
        ImGui::DragFloat3("", (float*)&Entity, SpeedIn);
        break;
    }
    }
    ImGui::PopID();
}
