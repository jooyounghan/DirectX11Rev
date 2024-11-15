#include "ViewportWindow.h"

#include "ACamera.h"
#include "Ray.h"

#include "UIVariable.h"


using namespace std;
using namespace ImGui;
using namespace DirectX;

ViewportWindow::ViewportWindow()
{
}

ViewportWindow::~ViewportWindow()
{
}

void ViewportWindow::RenderWindow()
{
    Begin("Viewport");

    if (CurrentCamera != nullptr)
    {
        ImagePosition = ImGui::GetCursorScreenPos();
        ImageSize = GetContentRegionAvail();


        Image(CurrentCamera->GetSDRSceneSRV(), ImageSize);
        if (BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = AcceptDragDropPayload(DragDrop::GAsset))
            {
                IM_ASSERT(payload->DataSize == sizeof(AAssetFile*));
                AAssetFile* AssetFile = *(AAssetFile**)(payload->Data);

                ImGuiIO& io = GetIO();
                ImVec2 AbsMousePos = io.MousePos;
                ImVec2 RelativeMousePos = ImVec2(AbsMousePos.x - ImagePosition.x, AbsMousePos.y - ImagePosition.y);

                Ray ClickedRay = Ray::CreateRay(
                    RelativeMousePos.x, RelativeMousePos.y,
                    ImageSize.x, ImageSize.y,
                    CurrentCamera->GetProjectionMatrix(),
                    CurrentCamera->GetViewMatrix()
                );

                const XMVECTOR PlacePositon = ClickedRay.Origin + ClickedRay.Direction * (500.f);

                AssetDropEvent.Invoke(AssetFile, PlacePositon.m128_f32[0], PlacePositon.m128_f32[1], PlacePositon.m128_f32[2]);
                
            }
            EndDragDropTarget();
        }
            
        if (IsMouseClicked(ImGuiMouseButton_Left) && IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
        {
            ImGuiIO& io = GetIO();
            ImVec2 AbsMousePos = io.MousePos;
            ImVec2 RelativeMousePos = ImVec2(AbsMousePos.x - ImagePosition.x, AbsMousePos.y - ImagePosition.y);

            UINT SelectedID = CurrentCamera->GetID(RelativeMousePos.x, RelativeMousePos.y, ImageSize.x, ImageSize.y);
            IDSelectEvent.Invoke(SelectedID);
        }
    }
    End();
}