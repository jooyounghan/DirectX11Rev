#include "ViewportWindow.h"

#include "EditorWorld.h"
#include "GameWorld.h"
#include "MapAsset.h"

#include "EditorActor.h"
#include "EditorCamera.h"

#include "AssetManager.h"
#include "AAssetFile.h"

#include "MapAsset.h"
#include "GlobalVariable.h"

#include "IIntersectable.h"
#include "EditorWorld.h"

using namespace std;
using namespace ImGui;

ViewportWindow::ViewportWindow(EditorWorld* EditorWorldIn)
    : IEditorLinkedWindow(EditorWorldIn)
{
    EditorActor* EditorActorInstance= EditorWorldCached->GetEditorActorInstance();
    assert(EditorActorInstance != nullptr);

    EditorCameraCached = EditorActorInstance->GetEditorCameraCached();
    assert(EditorCameraCached != nullptr);
}

ViewportWindow::~ViewportWindow()
{
}

void ViewportWindow::RenderWindow()
{
    Begin("Viewport");
    MapAsset* CurrentMap = GameWorldCached->GetCurrentMap();
    if (CurrentMap != nullptr)
    {
        if (EditorCameraCached != nullptr)
        {
            Text(CurrentMap->GetAssetName().c_str());
            ImagePosition = GetCursorScreenPos();
            ImageSize = GetContentRegionAvail();
            Image(EditorCameraCached->GetResolvedSceneSRV(), ImageSize);
            ManageAssetDrop(CurrentMap);
            ManageMouseLBClick(CurrentMap);
        }
    }
    End();
}

void ViewportWindow::ManageAssetDrop(MapAsset* CurrentMap)
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
                switch (AssetFile->GetAssetType())
                {
                case EAssetType::StaticMesh:
                case EAssetType::SkeletalMesh:
                {
                    Ray ClickedRay = Ray::CreateRay(
                        RelativeMousePos.x, RelativeMousePos.y, 
                        ImageSize.x, ImageSize.y,
                        EditorCameraCached->GetProjectionMatrix(), 
                        EditorCameraCached->GetViewMatrix()
                    );
                    const XMVECTOR PlacePositon = ClickedRay.Origin + ClickedRay.Direction * (500.f);

                    CurrentMap->AddRenderObject(
                        (AMeshAsset*)AssetFile,
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

void ViewportWindow::ManageMouseLBClick(MapAsset* CurrentMap)
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
