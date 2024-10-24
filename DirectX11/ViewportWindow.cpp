#include "ViewportWindow.h"

#include "EditorWorld.h"
#include "GameWorld.h"

#include "MapAsset.h"
#include "AssetManager.h"

#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"

#include "EditorPawn.h"
#include "ACamera.h"

#include "UIVariable.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;

ViewportWindow::ViewportWindow(EditorWorld* EditorWorldCached)
    : EditorWorldCached(EditorWorldCached)
{

    if (EditorWorldCached != nullptr)
    {
        GameWorldCached = EditorWorldCached->GetGameWorldCached();
        if (GameWorldCached != nullptr)
        {
            AssetManagerCached = GameWorldCached->GetAssetManagerInstance();
            CurrentMap = GameWorldCached->GetCurrentMap();
            if (CurrentMap != nullptr)
            {
                CameraCached = CurrentMap->GetCurrentCamera();
            }
        }
    }
}

ViewportWindow::~ViewportWindow()
{
}

void ViewportWindow::RenderWindow()
{
    Begin("Viewport");
    CurrentMap = GameWorldCached->GetCurrentMap();
    if (CurrentMap != nullptr)
    {
        if (CameraCached != nullptr)
        {
            ImagePosition = ImGui::GetCursorScreenPos();
            ImageSize = GetContentRegionAvail();
            Image(CameraCached->GetSDRSceneSRV(), ImageSize);
            ManageAssetDrop();
            ManageMouseLBClick();
        }
        else
        {
            CameraCached = CurrentMap->GetCurrentCamera();
        }
    }
    End();
}

void ViewportWindow::ManageAssetDrop()
{
    if (BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = AcceptDragDropPayload(DragDrop::GAsset))
        {
            IM_ASSERT(payload->DataSize == sizeof(AAssetFile*));
            AAssetFile* AssetFile = *(AAssetFile**)(payload->Data);

            if (CurrentMap != nullptr)
            {
                ImGuiIO& io = GetIO();
                ImVec2 AbsMousePos = io.MousePos;
                ImVec2 RelativeMousePos = ImVec2(AbsMousePos.x - ImagePosition.x, AbsMousePos.y - ImagePosition.y);

                Ray ClickedRay = Ray::CreateRay(
                    RelativeMousePos.x, RelativeMousePos.y, 
                    ImageSize.x, ImageSize.y,
                    CameraCached->GetProjectionMatrix(),
                    CameraCached->GetViewMatrix()
                );
                const XMVECTOR PlacePositon = ClickedRay.Origin + ClickedRay.Direction * (500.f);
                
                const string& AssetType = AssetFile->GetAssetType();
                if (AssetType == StaticMeshAsset::StaticMeshAssetKind)
                {
                    CurrentMap->AddStaticMeshObjectActor(
                        AssetManagerCached->GetManagingStaticMesh(AssetFile->GetAssetName()),
                        PlacePositon.m128_f32[0],
                        PlacePositon.m128_f32[1],
                        PlacePositon.m128_f32[2]
                    );
                }
                else if (AssetType == SkeletalMeshAsset::SkeletalMeshAssetKind)
                {
                    CurrentMap->AddSkeletalMeshObjectActor(
                        AssetManagerCached->GetManagingSkeletalMesh(AssetFile->GetAssetName()),
                        PlacePositon.m128_f32[0],
                        PlacePositon.m128_f32[1],
                        PlacePositon.m128_f32[2]
                    );
                }
            }
        }
        EndDragDropTarget();
    }
}

void ViewportWindow::ManageMouseLBClick()
{
    if (IsMouseClicked(ImGuiMouseButton_Left) && IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
    {
        ImGuiIO& io = GetIO();
        ImVec2 AbsMousePos = io.MousePos;
        ImVec2 RelativeMousePos = ImVec2(AbsMousePos.x - ImagePosition.x, AbsMousePos.y - ImagePosition.y);

        UINT SelectedID = CameraCached->GetID(RelativeMousePos.x, RelativeMousePos.y, ImageSize.x, ImageSize.y);
        EditorWorldCached->SetSelecteObjectByID(SelectedID);
    }
}
