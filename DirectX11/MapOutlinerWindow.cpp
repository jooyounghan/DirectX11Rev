#include "MapOutlinerWindow.h"
#include "ModelDetailedInformationVisitor.h"

#include "GlobalVariable.h"
#include "AssetManager.h"
#include "MapAsset.h"

#include <list> 
#include <memory>

using namespace std;
using namespace ImGui;

MapOutlinerWindow::MapOutlinerWindow()
	: 
    AddAttachableModalInstance("Add Attachable Object"),
    AddPlaceableModalInstance("Add Placeable Object"),
    DeleteAttachableModalInstance("Delete Attachable Object"),
    DeletePlaceableModalInstance("Delete Placeable Object")
{

}

MapOutlinerWindow::~MapOutlinerWindow()
{
}

void MapOutlinerWindow::RenderWindow()
{
    RenderMapOutliner();
    RenderModelDetail();
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
        APlaceableObject* SelectedPlaceable = SelectedMapAsset->GetSelectedPlaceable();
        AAttachableObject* SelectedAttached = SelectedMapAsset->GetSelectedAttachable();

        ModelDetailedInformationVisitor InformationVisitor;

        if (SelectedAttached != nullptr)
        {
            SelectedAttached->AcceptGui(&InformationVisitor);
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
            AddAttachableModalInstance.SetCurrentMapAssetCached(Map);
            AddPlaceableModalInstance.SetCurrentMapAssetCached(Map);
            DeleteAttachableModalInstance.SetCurrentMapAssetCached(Map);
            DeletePlaceableModalInstance.SetCurrentMapAssetCached(Map);

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

    APlaceableObject* SelectedPlacedObject = SelectedMapAsset->GetSelectedPlaceable();
    const list<unique_ptr<APlaceableObject>>& RootPlaceables = SelectedMapAsset->GetRootPlaceables();

    for (auto& Placeable : RootPlaceables)
    {
        APlaceableObject* PlacedObject = Placeable.get();

        ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (SelectedPlacedObject == PlacedObject)
        {
            NodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        bool IsOpen = TreeNodeEx(PlacedObject->GetObjectName().c_str(), NodeFlags);
        if (IsItemClicked() or IsItemToggledOpen())
        {
            SelectedMapAsset->SetSelectedPlaceable(PlacedObject);
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

    APlaceableObject* SelectedPlacedObject = SelectedMapAsset->GetSelectedPlaceable();
    if (SelectedPlacedObject != nullptr)
    {
        const list<unique_ptr<AAttachableObject>>& AttachedObjects = SelectedPlacedObject->GetAttachedChildrenObjects();

        ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth;
        if (AttachedObjects.size() == 0)
        {
            NodeFlags |= ImGuiTreeNodeFlags_Leaf;
        }
        else
        {
            NodeFlags |= (ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow);
        }

        bool IsOpen = TreeNodeEx(SelectedPlacedObject->GetObjectName().c_str(), NodeFlags);
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

    DeleteAttachableModalInstance.DoModal();
}

void MapOutlinerWindow::RenderAttachedOutline(AAttachableObject* Attachment)
{
    AAttachableObject* SelectedAttachableObject = SelectedMapAsset->GetSelectedAttachable();
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

    if (SelectedAttachableObject == Attachment)
    {
        NodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    bool IsOpen = TreeNodeEx(Attachment->GetObjectName().c_str(), NodeFlags);
    if (IsItemClicked() or IsItemToggledOpen())
    {
        SelectedMapAsset->SetSelectedAttachable(Attachment);
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
