#include "TransformationInformationDrawer.h"
#include "AObject.h"

#include "GlobalVariable.h"

#include <string>
#include <format>
#include <functional>

using namespace std;
using namespace ImGui;
using namespace DirectX;

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

    XMFLOAT3 DummyPosition;
    XMFLOAT3 DummyAngle;
    XMFLOAT3 DummyScale;
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

            XMFLOAT3 ObjectAbsolutePosition = ObjectCached->GetAbsolutePosition();
            XMFLOAT3 ParentAbsolutePosition = ParentObjectCached->GetAbsolutePosition();

            XMVECTOR vObjectAbsolutePosition = XMLoadFloat3(&ObjectAbsolutePosition);
            XMVECTOR vParentAbsolutePosition = XMLoadFloat3(&ParentAbsolutePosition);

            ImGui::DragFloat3("", (float*)&vObjectAbsolutePosition, 1.f);

            XMVECTOR vDeltaAbsolutePosition = vObjectAbsolutePosition - vParentAbsolutePosition;

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

            XMStoreFloat3(&ObjectCached->RelativePosition, XMVector3Transform(vDeltaAbsolutePosition, CoordinateInvTransformation));
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
            XMFLOAT3 ObjectAbsoluteAngle = ObjectCached->GetAbsoluteAngle();
            XMFLOAT3 ParentAbsoluteAngle = ParentObjectCached->GetAbsoluteAngle();

            ImGui::DragFloat3("", (float*)&ObjectAbsoluteAngle, 1.f);

            ObjectCached->RelativeAngle = XMFLOAT3{
                ObjectAbsoluteAngle.x - ParentAbsoluteAngle.x,
                ObjectAbsoluteAngle.y - ParentAbsoluteAngle.y,
                ObjectAbsoluteAngle.z - ParentAbsoluteAngle.z,
            };
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
            XMFLOAT3 ObjectAbsoluteScale = ObjectCached->GetAbsoluteScale();
            XMFLOAT3 ParentAbsoluteScale = ParentObjectCached->GetAbsoluteScale();

            ImGui::DragFloat3("", (float*)&ObjectAbsoluteScale, 0.01f);

            ObjectCached->RelativeScale = XMFLOAT3{
                ObjectAbsoluteScale.x / ParentAbsoluteScale.x,
                ObjectAbsoluteScale.y / ParentAbsoluteScale.y,
                ObjectAbsoluteScale.z / ParentAbsoluteScale.z,
            };
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

