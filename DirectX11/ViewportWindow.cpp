#include "ViewportWindow.h"

#include "EditorWorld.h"
#include "GameWorld.h"
#include "MapAsset.h"

#include "EditorPawn.h"
#include "EditorCamera.h"

#include "AssetManager.h"
#include "AAssetFile.h"
#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"

#include "MapAsset.h"
#include "GlobalVariable.h"

#include "IIntersectable.h"
#include "EditorWorld.h"

using namespace std;
using namespace ImGui;

ViewportWindow::ViewportWindow(EditorWorld* EditorWorldIn)
    : EditorWorldCached(EditorWorldIn)
{
    if (EditorWorldCached != nullptr)
    {
        EditorActorCached  = EditorWorldCached->GetEditorActorInstance();
        if (EditorActorCached != nullptr)
        {
            EditorCameraCached = EditorActorCached->GetEditorCameraCached();
        }

        GameWorldCached = EditorWorldCached->GetGameWorldCached();

        if (GameWorldCached != nullptr)
        {
            AssetManagerCached = GameWorldCached->GetAssetManagerInstance();
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
        if (EditorCameraCached != nullptr)
        {
            Text(CurrentMap->GetAssetName().c_str());
            ImagePosition = GetCursorScreenPos();
            ImageSize = GetContentRegionAvail();
            Image(EditorCameraCached->GetSceneSRV(), ImageSize);
            ManageAssetDrop();
            ManageMouseLBClick();
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
                    EditorCameraCached->GetProjectionMatrix(), 
                    EditorCameraCached->GetViewMatrix()
                );
                const XMVECTOR PlacePositon = ClickedRay.Origin + ClickedRay.Direction * (500.f);
                
                switch (AssetFile->GetAssetType())
                {
                case EAssetType::StaticMesh:
                {
                    CurrentMap->AddStaticMeshObjectActor(
                        AssetManagerCached->GetManagingStaticMesh(AssetFile->GetAssetName()),
                        PlacePositon.m128_f32[0],
                        PlacePositon.m128_f32[1],
                        PlacePositon.m128_f32[2]
                    );
                    break;
                }
                case EAssetType::SkeletalMesh:
                {
                    CurrentMap->AddSkeletalMeshObjectActor(
                        AssetManagerCached->GetManagingSkeletalMesh(AssetFile->GetAssetName()),
                        PlacePositon.m128_f32[0],
                        PlacePositon.m128_f32[1],
                        PlacePositon.m128_f32[2]
                    );
                    break;
                }
                default:
                    break;
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

        UINT SelectedID = EditorCameraCached->GetID(RelativeMousePos.x, RelativeMousePos.y, ImageSize.x, ImageSize.y);
        EditorWorldCached->SetSelecteObjectByID(SelectedID);
    }
}
