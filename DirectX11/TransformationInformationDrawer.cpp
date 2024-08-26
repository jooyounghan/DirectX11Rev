#include "TransformationInformationDrawer.h"
#include "AObject.h"

#include "GlobalVariable.h"
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

TransformationInformationDrawer::TransformationInformationDrawer(AObject* CurrentObject, AObject* ParentObject)
    : AInformationDrawer(CurrentObject), ParentObjectCached(ParentObject)
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
    DrawPositionEntity(SelectedTranslationIndex);

    DrawEntitySelection("Rotation", SelectedRotationIndex);
    SameLine();
    DrawAngleEntity(SelectedRotationIndex);

    DrawEntitySelection("Scale", SelectedScaleIndex);
    SameLine();
    DrawScaleEntity(SelectedScaleIndex);
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

void TransformationInformationDrawer::DrawPositionEntity(const ETransfomationSelect& PositionSelectedIndex)
{
    ImGui::PushID(ObjectCached->GetRelativePositionID().c_str());



    switch (PositionSelectedIndex)
    {
    case ETransfomationSelect::Absolute:
    {
        if (ParentObjectCached)
        {
            XMVECTOR Translation;
            XMVECTOR Rotation;
            XMVECTOR Scaling;

            SPosition4D ObjectAbsolutePosition = ObjectCached->GetAbsolutePosition();
            SPosition4D ParentAbsolutePosition = ParentObjectCached->GetAbsolutePosition();

            ImGui::DragFloat3("", (float*)&ObjectAbsolutePosition, 1.f);

            SPosition4D DeltaAbsolutePosition = ObjectAbsolutePosition - ParentAbsolutePosition;
            DeltaAbsolutePosition.w = 1.f;

            const XMMATRIX ParentTransformation = ParentObjectCached->GetTransformation();
            XMMatrixDecompose(&Scaling, &Rotation, &Translation, ParentTransformation);

            XMVECTOR CurrentForward = XMVector3Rotate(Direction::GDefaultForward, Rotation);
            XMVECTOR CurrentUp = XMVector3Rotate(Direction::GDefaultUp, Rotation);
            XMVECTOR CurrentRight = XMVector3Rotate(Direction::GDefaultRight, Rotation);

            XMMATRIX CoordinateInvTransformation = XMMatrixInverse(nullptr, XMMatrixSet(
                CurrentRight.m128_f32[0], CurrentRight.m128_f32[1], CurrentRight.m128_f32[2], CurrentRight.m128_f32[3],
                CurrentUp.m128_f32[0], CurrentUp.m128_f32[1], CurrentUp.m128_f32[2], CurrentUp.m128_f32[3],
                CurrentForward.m128_f32[0], CurrentForward.m128_f32[1], CurrentForward.m128_f32[2], CurrentForward.m128_f32[3],
                0.f, 0.f, 0.f, 1.f)
            );

            ObjectCached->RelativePosition.Position = XMVector3Transform(DeltaAbsolutePosition.Position, CoordinateInvTransformation);
        }
        else
        {
            ImGui::DragFloat3("", (float*)&ObjectCached->RelativePosition, 1.f);
        }
        break;
    }
    case ETransfomationSelect::Relative:
    {
        ImGui::DragFloat3("", (float*)&ObjectCached->RelativePosition, 1.f);
        break;
    }
    }
    ImGui::PopID();
}

void TransformationInformationDrawer::DrawAngleEntity(const ETransfomationSelect& PositionSelectedIndex)
{
    ImGui::PushID(ObjectCached->GetRelativeAngleID().c_str());

    switch (PositionSelectedIndex)
    {
    case ETransfomationSelect::Absolute:
    {
        if (ParentObjectCached)
        {
            SAngle ObjectAbsoluteAngle = ObjectCached->GetAbsoluteAngle();
            SAngle ParentAbsoluteAngle = ParentObjectCached->GetAbsoluteAngle();

            ImGui::DragFloat3("", (float*)&ObjectAbsoluteAngle, 1.f);

            ObjectCached->RelativeAngle = ObjectAbsoluteAngle - ParentAbsoluteAngle;
        }
        else
        {
            ImGui::DragFloat3("", (float*)&ObjectCached->RelativeAngle, 0.1f);
        }
        break;
    }
    case ETransfomationSelect::Relative:
    {
        ImGui::DragFloat3("", (float*)&ObjectCached->RelativeAngle, 0.1f);
        break;
    }
    }
    ImGui::PopID();
}

void TransformationInformationDrawer::DrawScaleEntity(const ETransfomationSelect& PositionSelectedIndex)
{
    ImGui::PushID(ObjectCached->GetRelativeScaleID().c_str());

    switch (PositionSelectedIndex)
    {
    case ETransfomationSelect::Absolute:
    {
        if (ParentObjectCached)
        {
            SAngle ObjectAbsoluteScale = ObjectCached->GetAbsoluteScale();
            SAngle ParentAbsoluteScale = ParentObjectCached->GetAbsoluteScale();

            ImGui::DragFloat3("", (float*)&ObjectAbsoluteScale, 1.f);

            ObjectCached->RelativeScale = ObjectAbsoluteScale / ParentAbsoluteScale;
        }
        else
        {
            ImGui::DragFloat3("", (float*)&ObjectCached->RelativeScale, 0.01f);
        }
        break;
    }
    case ETransfomationSelect::Relative:
    {
        ImGui::DragFloat3("", (float*)&ObjectCached->RelativeScale, 0.01f);
        break;
    }
    }
    ImGui::PopID();
}

