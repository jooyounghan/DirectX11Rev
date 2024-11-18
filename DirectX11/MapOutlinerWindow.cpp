#include "MapOutlinerWindow.h"
#include "ModelDetailedInformationVisitor.h"

#include "GlobalVariable.h"
#include "UIVariable.h"

#include "AssetManager.h"
#include "MapAsset.h"

#include "ACamera.h"
#include "Ray.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;

MapOutlinerWindow::MapOutlinerWindow()
	: 
    AddAttachableModalInstance(this, "Add Attachable Object"),
    AddPlaceableModalInstance("Add Placeable Object"),
    DeleteAttachableModalInstance(this, "Delete Attachable Object"),
    DeletePlaceableModalInstance(this, "Delete Placeable Object")
{

}

MapOutlinerWindow::~MapOutlinerWindow()
{
}

void MapOutlinerWindow::RenderWindow()
{
    RenderViewport();
    RenderMapOutliner();
    RenderModelDetail();
}

void MapOutlinerWindow::RenderViewport()
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

    Begin("Deffered Shading");
    if (CurrentCamera != nullptr)
    {
        ImVec2 DefferedShadingRegion = GetContentRegionAvail();

        Image(CurrentCamera->GetGBufferSRV(EGBuffer::Position_GBuffer), ImVec2(DefferedShadingRegion.x / 2.f, DefferedShadingRegion.y / 3.f));
        SameLine();
        Image(CurrentCamera->GetGBufferSRV(EGBuffer::BaseColor_GBuffer), ImVec2(DefferedShadingRegion.x / 2.f, DefferedShadingRegion.y / 3.f));
        
        Image(CurrentCamera->GetGBufferSRV(EGBuffer::Normal_GBuffer), ImVec2(DefferedShadingRegion.x / 2.f, DefferedShadingRegion.y / 3.f));
        SameLine();
        Image(CurrentCamera->GetGBufferSRV(EGBuffer::AO_Metallic_Roughness_GBuffer), ImVec2(DefferedShadingRegion.x / 2.f, DefferedShadingRegion.y / 3.f));
        
        Image(CurrentCamera->GetGBufferSRV(EGBuffer::Emissive_GBuffer), ImVec2(DefferedShadingRegion.x / 2.f, DefferedShadingRegion.y / 3.f));
        SameLine();
        //Image(CurrentCamera->GetGBufferDSV(), ImVec2(DefferedShadingRegion.x / 2.f, DefferedShadingRegion.y / 3.f));
    }
    End();
}

void MapOutlinerWindow::RenderMapOutliner()
{
    Begin("Map Outliner");
    RenderMapAssetOverview();
    if (SelectedMapAsset)
    {
        RenderPlaceablesOutline();
        RenderSelectedPlaceableOutline();
    }
    End();
}

void MapOutlinerWindow::RenderModelDetail()
{
    Begin("Model Detailed");
    ImVec2 RegionAvail = GetContentRegionAvail();
    BeginChild("Selected Detail Information", ImVec2(RegionAvail.x, RegionAvail.y), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);

    if (SelectedMapAsset)
    {
        ModelDetailedInformationVisitor InformationVisitor;

        if (SelectedAttachable != nullptr)
        {
            SelectedAttachable->AcceptGui(&InformationVisitor);
        }
        else if (SelectedPlaceable != nullptr)
        {
            SelectedPlaceable->AcceptGui(&InformationVisitor);
        }
        else
        {
            SelectedMapAsset->AcceptGui(&InformationVisitor);
        }
    }
    EndChild();
    End();
}

void MapOutlinerWindow::RenderMapAssetOverview()
{
    Text("Map Asset Overview");
    const unordered_map<string, shared_ptr<MapAsset>>& ManaginMaps = App::GAssetManager->GetManagingMaps();

    for (auto& ManagingMap : ManaginMaps)
    {
        const string& MapName = ManagingMap.first;
        const std::shared_ptr<MapAsset>& Map = ManagingMap.second;

        ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (SelectedMapAsset == Map)
        {
            NodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        bool IsOpen = TreeNodeEx(MapName.c_str(), NodeFlags);
        if (IsItemClicked() or IsItemToggledOpen())
        {
            SelectedMapAsset = Map;
            AddPlaceableModalInstance.SetMapAssetCached(Map);
            DeletePlaceableModalInstance.SetMapAssetCached(Map);

            MapSelectedEvent.Invoke(Map);
        }

        if (IsOpen)
        {
            TreePop();
        }
    }

}

void MapOutlinerWindow::RenderPlaceablesOutline()
{
    Text("Placed Object List:");
    AddPlaceableModalInstance.DoModal();

    const list<unique_ptr<APlaceableObject>>& RootPlaceables = SelectedMapAsset->GetRootPlaceables();

    for (auto& Placeable : RootPlaceables)
    {
        APlaceableObject* PlacedObject = Placeable.get();

        ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (SelectedPlaceable == PlacedObject)
        {
            NodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        bool IsOpen = TreeNodeEx(PlacedObject->GetObjectName().c_str(), NodeFlags);
        if (IsItemClicked() or IsItemToggledOpen())
        {
            SelectedPlaceable = PlacedObject;
            SelectedAttachable = nullptr;
        }

        if (IsOpen)
        {
            TreePop();
        }
    }

    DeletePlaceableModalInstance.DoModal();
}

void MapOutlinerWindow::RenderSelectedPlaceableOutline()
{
    Text("Attached Object List:\t");
    AddAttachableModalInstance.DoModal();

    if (SelectedPlaceable != nullptr)
    {
        const list<unique_ptr<AAttachableObject>>& AttachedObjects = SelectedPlaceable->GetAttachedChildrenObjects();

        ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth;
        if (AttachedObjects.size() == 0)
        {
            NodeFlags |= ImGuiTreeNodeFlags_Leaf;
        }
        else
        {
            NodeFlags |= (ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow);
        }

        Text(SelectedPlaceable->GetObjectName().c_str());
        Indent();
        for (auto& AttachedObject : AttachedObjects)
        {
            RenderAttachedOutline(AttachedObject.get());
        }
        Unindent();
    }

    DeleteAttachableModalInstance.DoModal();
}

void MapOutlinerWindow::RenderAttachedOutline(AAttachableObject* Attachment)
{
    const list<unique_ptr<AAttachableObject>>& AttachedObjects = Attachment->GetAttachedChildrenObjects();

    ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth;
    if (AttachedObjects.size() == 0)
    {
        NodeFlags |= ImGuiTreeNodeFlags_Leaf;
    }
    else
    {
        NodeFlags |= (ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow);
    }

    if (SelectedAttachable == Attachment)
    {
        NodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    bool IsOpen = TreeNodeEx(Attachment->GetObjectName().c_str(), NodeFlags);
    if (IsItemClicked() or IsItemToggledOpen())
    {
        SelectedAttachable = Attachment;
    }

    if (IsOpen)
    {
        Indent();
        for (auto& AttachedObject : AttachedObjects)
        {
            RenderAttachedOutline(AttachedObject.get());
        }
        Unindent();
        TreePop();
    }
}
