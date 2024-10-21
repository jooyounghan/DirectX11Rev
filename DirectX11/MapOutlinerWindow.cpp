#include "MapOutlinerWindow.h"

#include "EditorWorld.h"
#include "GameWorld.h"
#include "MapAsset.h"

#include <list> 
#include <memory>

using namespace std;
using namespace ImGui;

MapOutlinerWindow::MapOutlinerWindow(EditorWorld* EditorWorldIn)
	: 
    EditorWorldCached(EditorWorldIn), 
    AddAttachableModalInstance("Add Attachable Object", EditorWorldIn),
    AddPlaceableModalInstance("Add Placeable Object", EditorWorldIn),
    DeleteAttachableModalInstance("Delete Attachable Object", EditorWorldCached),
    DeletePlaceableModalInstance("Delete Placeable Object", EditorWorldCached)
{
    if (EditorWorldCached != nullptr)
    {
        GameWorldCached = EditorWorldCached->GetGameWorldCached();
    }
}

MapOutlinerWindow::~MapOutlinerWindow()
{
}

void MapOutlinerWindow::RenderWindow()
{
    Begin("Map Outliner");

    CurrentMap = GameWorldCached->GetCurrentMap();

    AddAttachableModalInstance.SetCurrentMapAssetCached(CurrentMap);
    AddPlaceableModalInstance.SetCurrentMapAssetCached(CurrentMap);
    DeleteAttachableModalInstance.SetCurrentMapAssetCached(CurrentMap);
    DeletePlaceableModalInstance.SetCurrentMapAssetCached(CurrentMap);

    RenderMapAssetOverview();
    RenderPlaceablesOutline();
    RenderSelectedPlaceableOutline();

    End();
}

void MapOutlinerWindow::RenderMapAssetOverview()
{
    Text("Map Asset Overview");
    RenderMapInformation();

}

void MapOutlinerWindow::RenderPlaceablesOutline()
{
    Text("Placed Object List:");
    AddPlaceableModalInstance.DoModal();

    RenderPlacedListBox();

    DeletePlaceableModalInstance.DoModal();
}

void MapOutlinerWindow::RenderSelectedPlaceableOutline()
{
    Text("Attached Object List:\t");
    AddAttachableModalInstance.DoModal();
    
    RenderAttachedTree();

    DeleteAttachableModalInstance.DoModal();
}

void MapOutlinerWindow::RenderMapInformation()
{
    const unordered_map<UINT, shared_ptr<MapAsset>>& MapInstances = GameWorldCached->GetMapInstances();
    for (auto& MapInstance : MapInstances)
    {
        MapAsset* MapAssetPointer = MapInstance.second.get();

        ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;

        if (EditorWorldCached->GetSelectedMapAsset() == MapAssetPointer)
        {
            NodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        bool IsOpen = TreeNodeEx(MapAssetPointer->GetAssetName().c_str(), NodeFlags);
        if (IsItemClicked() or IsItemToggledOpen())
        {
            EditorWorldCached->SetSelectedMapAsset(MapAssetPointer);
        }

        if (IsOpen)
        {
            TreePop();
        }
    }
}

void MapOutlinerWindow::RenderPlacedListBox()
{
    const list<unique_ptr<APlaceableObject>>& RootPlaceables = CurrentMap->GetRootPlaceables();
    for (auto& Placeable : RootPlaceables)
    {
        APlaceableObject* PlaceableObj = Placeable.get();

        ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;

        if (EditorWorldCached->GetSelectedPlaceable() == PlaceableObj)
        {
            NodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        bool IsOpen = TreeNodeEx(PlaceableObj->GetObjectName().c_str(), NodeFlags);
        if (IsItemClicked() or IsItemToggledOpen())
        {
            EditorWorldCached->SetSelectedPlaceable(PlaceableObj);
        }

        if (IsOpen)
        {
            TreePop();
        }
    }

}

void MapOutlinerWindow::RenderAttachedTree()
{
    APlaceableObject* SelectedPlaceable = EditorWorldCached->GetSelectedPlaceable();
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

        bool IsOpen = TreeNodeEx(SelectedPlaceable->GetObjectName().c_str(), NodeFlags);
        if (IsItemClicked() or IsItemToggledOpen())
        {
            EditorWorldCached->SetSelectedAttached(nullptr);
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

    if (EditorWorldCached->GetSelectedAttached() == Attachment)
    {
        NodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    bool IsOpen = TreeNodeEx(Attachment->GetObjectName().c_str(), NodeFlags);
    if (IsItemClicked() or IsItemToggledOpen())
    {
        EditorWorldCached->SetSelectedAttached(Attachment);
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
