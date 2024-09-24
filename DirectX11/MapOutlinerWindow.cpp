#include "MapOutlinerWindow.h"

#include "AddPlaceableObjectHelper.h"
#include "AddAttachableObjectHelper.h"

#include "EditorWorld.h"
#include "GameWorld.h"
#include "MapAsset.h"

#include <list> 
#include <memory>

using namespace std;
using namespace ImGui;

const char* MapOutlinerWindow::AddMapButtonID = "Add Map +";
const char* MapOutlinerWindow::AddPlaceableButtonID = "Add Placeable +";
const char* MapOutlinerWindow::AddAttachableButtonID = "Add Attacheable +";

const char* MapOutlinerWindow::DeleteMapModalID = "Delete Map";
const char* MapOutlinerWindow::DeletePlaceableModalID = "Delete Placeable Object";
const char* MapOutlinerWindow::DeleteAttachableModalID = "Delete Attachable Object";

const char* MapOutlinerWindow::AddMapModalID = "Add Map";
const char* MapOutlinerWindow::AddPlaceableModalID = "Add Placeable Object";
const char* MapOutlinerWindow::AddAttachableModalID = "Add Attachable Object";

MapOutlinerWindow::MapOutlinerWindow(EditorWorld* EditorWorldIn)
	: EditorWorldCached(EditorWorldIn)
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

    RenderMapAssetOverview();
    RenderPlaceablesOutline();
    RenderSelectedPlaceableOutline();

    End();
}

void MapOutlinerWindow::RenderMapAssetOverview()
{
    Text("Map Asset Overview");
    SameLine();
    DoModalAddNewMap();

    RenderMapInformation();
    DoModalDeleteMap();
}

void MapOutlinerWindow::RenderPlaceablesOutline()
{
    Text("Placed Object List:");
    SameLine();
    DoModalAddPlaceableObject();

    RenderPlacedListBox();
    DoModalDeletePlaceableObject();
}

void MapOutlinerWindow::RenderSelectedPlaceableOutline()
{
    Text("Attached Object List:\t");
    SameLine();
    DoModalAddAttachableObject();
    
    RenderAttachedTree();
    DoModalDeleteAttachableObject();
}

void MapOutlinerWindow::DoModalDeleteMap()
{
}

void MapOutlinerWindow::DoModalDeletePlaceableObject()
{
    if (IsKeyPressed(ImGuiKey::ImGuiKey_Delete) && IsWindowFocused())
    {
        OpenPopup(DeletePlaceableModalID);
    }

    SetWindowPosToCenter();
    APlaceableObject* SelectedPlaceable = EditorWorldCached->GetSelectedPlaceable();
    if (SelectedPlaceable != nullptr)
    {
        if (BeginPopupModal(DeletePlaceableModalID, NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            Text("Are you sure for deleting %s ?", SelectedPlaceable->GetObjectName().c_str());
            Separator();

            if (Button("OK", ImVec2(120, 0))) 
            { 
                if (CurrentMap != nullptr)
                {
                    CurrentMap->PlaceableDeleteHelper(SelectedPlaceable);
                    EditorWorldCached->SetSelectedPlaceable(nullptr);
                }
                CloseCurrentPopup(); 
            }
            SetItemDefaultFocus();
            SameLine();
            if (Button("Cancel", ImVec2(120, 0))) 
            { 
                CloseCurrentPopup(); 
            }
            EndPopup();
        }
    }
    ResetWindowPosToPrevious();
}

void MapOutlinerWindow::DoModalDeleteAttachableObject()
{
    if (IsKeyPressed(ImGuiKey::ImGuiKey_Delete) && IsWindowFocused())
    {
        OpenPopup(DeleteAttachableModalID);
    }

    SetWindowPosToCenter();
    AAttachableObject* SelectedAttached = EditorWorldCached->GetSelectedAttached();
    if (SelectedAttached != nullptr)
    {
        if (BeginPopupModal(DeleteAttachableModalID, NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            Text("Are you sure for Deleting %s ?", SelectedAttached->GetObjectName().c_str());
            Separator();

            if (Button("OK", ImVec2(120, 0))) 
            { 
                APlaceableObject* SelectedPlaceableObject = EditorWorldCached->GetSelectedPlaceable();
                if (SelectedPlaceableObject != nullptr)
                {
                    SelectedPlaceableObject->RemoveAttachedObject(SelectedAttached);
                    EditorWorldCached->SetSelectedAttached(nullptr);
                }
                CloseCurrentPopup(); 
            }
            SetItemDefaultFocus();
            SameLine();
            if (Button("Cancel", ImVec2(120, 0))) 
            { 
                CloseCurrentPopup(); 
            }
            EndPopup();
        }
    }
    ResetWindowPosToPrevious();
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

void MapOutlinerWindow::SetWindowPosToCenter()
{
    PreviousWindowPos = GetWindowPos();
    ImVec2 Center = GetMainViewport()->GetCenter();
    SetWindowPos(Center, ImGuiCond_Appearing);
}

void MapOutlinerWindow::ResetWindowPosToPrevious()
{
    SetWindowPos(PreviousWindowPos, ImGuiCond_Appearing);
}

void MapOutlinerWindow::DoModalAddNewMap()
{
    float ButtonWidth = CalcTextSize(AddMapButtonID).x;
    SetCursorPosX(GetCursorPosX() + GetContentRegionAvail().x - ButtonWidth);
    if (SmallButton(AddMapButtonID))
    {
        OpenPopup(AddMapModalID);
    }

    SetWindowPosToCenter();
    if (BeginPopupModal(AddMapModalID, NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static char MapNameBuffer[128] = "";
        ImGui::InputTextWithHint("Map Name", "Enter New Map Name Here", MapNameBuffer, IM_ARRAYSIZE(MapNameBuffer));

        if (Button("OK", ImVec2(120, 0))) { CloseCurrentPopup(); }
        SetItemDefaultFocus();
        SameLine();
        if (Button("Cancel", ImVec2(120, 0))) { CloseCurrentPopup(); }
        EndPopup();
    }
    ResetWindowPosToPrevious();
}

void MapOutlinerWindow::DoModalAddPlaceableObject()
{
    float ButtonWidth = CalcTextSize(AddPlaceableButtonID).x;
    SetCursorPosX(GetCursorPosX() + GetContentRegionAvail().x - ButtonWidth);
    if (SmallButton(AddPlaceableButtonID))
    {
        OpenPopup(AddPlaceableModalID);
    }

    SetWindowPosToCenter();
    if (BeginPopupModal(AddPlaceableModalID, NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        AddPlaceableObjectDrawer PlaceableAddPopupDrawer(EditorWorldCached, CurrentMap);
        PlaceableAddPopupDrawer.DrawAddObjectPopup();
        EndPopup();
    }
    ResetWindowPosToPrevious();
}

void MapOutlinerWindow::DoModalAddAttachableObject()
{
    float ButtonWidth = CalcTextSize(AddAttachableButtonID).x;
    SetCursorPosX(GetCursorPosX() + GetContentRegionAvail().x - ButtonWidth);
    if (SmallButton(AddAttachableButtonID))
    {
        OpenPopup(AddAttachableModalID);
    }

    SetWindowPosToCenter();
    if (BeginPopupModal(AddAttachableModalID, NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        AddAttachableObjectDrawer AttachableAddPopupDrawer(EditorWorldCached, CurrentMap);
        AttachableAddPopupDrawer.DrawAddObjectPopup();
        EndPopup();
    }
    ResetWindowPosToPrevious();
}


