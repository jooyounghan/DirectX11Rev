#include "ModelOutlinerWindow.h"

#include "AddPlaceableObjectHelper.h"
#include "AddAttachableObjectHelper.h"

#include "EditorWorld.h"
#include "GameWorld.h"
#include "Map.h"

#include <list> 
#include <memory>

using namespace std;
using namespace ImGui;

const char* ModelOutlinerWindow::AddPlaceableButtonID = "Add Placeable +";
const char* ModelOutlinerWindow::AddAttachableButtonID = "Add Attacheable +";

const char* ModelOutlinerWindow::DeletePlaceableModalID = "Delete Placeable Object";
const char* ModelOutlinerWindow::DeleteAttachableModalID = "Delete Attachable Object";

const char* ModelOutlinerWindow::AddPlaceableModalID = "Add Placeable Object";
const char* ModelOutlinerWindow::AddAttachableModalID = "Add Attachable Object";

ModelOutlinerWindow::ModelOutlinerWindow(EditorWorld* EditorWorldIn)
	: IEditorLinkedWindow(EditorWorldIn)
{
}

ModelOutlinerWindow::~ModelOutlinerWindow()
{
}

void ModelOutlinerWindow::RenderWindow()
{
    Begin("Map Outliner");

    CurrentMap = GameWorldCached->GetCurrentMap();

    RenderPlaceablesOutline();
    RenderSelectedPlaceableOutline();
    End();
}

void ModelOutlinerWindow::RenderPlaceablesOutline()
{
    RegionAvail = GetContentRegionAvail();
    BeginChild("Placeables Outliner", ImVec2(RegionAvail.x, RegionAvail.y * 0.5f));

    if (CurrentMap)
    {
        Text("Placed Object List:");
        SameLine();
        DoModalAddPlaceableObject();

        RenderPlacedListBox();
        DoModalDeletePlaceableObject();
    }
    EndChild();
}

void ModelOutlinerWindow::RenderSelectedPlaceableOutline()
{
    Text("Attached Object List:\t");
    SameLine();
    DoModalAddAttachableObject();
    
    RenderAttachedTree();
    DoModalDeleteAttachableObject();
}

void ModelOutlinerWindow::DoModalDeletePlaceableObject()
{
    if (IsKeyPressed(ImGuiKey::ImGuiKey_Delete) && IsWindowFocused())
    {
        OpenPopup(DeletePlaceableModalID);
    }

    SetWindowPosToCenter();
    PlaceableObject* SelectedPlaceable = EditorWorldCached->GetSelectedPlaceable();
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

void ModelOutlinerWindow::DoModalDeleteAttachableObject()
{
    if (IsKeyPressed(ImGuiKey::ImGuiKey_Delete) && IsWindowFocused())
    {
        OpenPopup(DeleteAttachableModalID);
    }

    SetWindowPosToCenter();
    AttachableObject* SelectedAttached = EditorWorldCached->GetSelectedAttached();
    if (SelectedAttached != nullptr)
    {
        if (BeginPopupModal(DeleteAttachableModalID, NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            Text("Are you sure for deleting %s ?", SelectedAttached->GetObjectName().c_str());
            Separator();

            if (Button("OK", ImVec2(120, 0))) 
            { 
                PlaceableObject* SelectedPlaceableObject = EditorWorldCached->GetSelectedPlaceable();
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

void ModelOutlinerWindow::RenderPlacedListBox()
{
    const list<unique_ptr<PlaceableObject>>& RootPlaceables = CurrentMap->GetRootPlaceables();
    for (auto& Placeable : RootPlaceables)
    {
        PlaceableObject* PlaceableObj = Placeable.get();

        ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;

        if (EditorWorldCached->GetSelectedPlaceable() == PlaceableObj)
        {
            NodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        bool IsOpen = TreeNodeEx(PlaceableObj->GetObjectName().c_str(), NodeFlags);
        if (IsItemClicked() or IsItemToggledOpen())
        {
            EditorWorldCached->SetSelectedPlaceable(PlaceableObj);
            EditorWorldCached->SetSelectedAttached(nullptr);
        }

        if (IsOpen)
        {
            TreePop();
        }
    }

}

void ModelOutlinerWindow::RenderAttachedTree()
{
    PlaceableObject* SelectedPlaceable = EditorWorldCached->GetSelectedPlaceable();
    if (SelectedPlaceable != nullptr)
    {
        const list<unique_ptr<AttachableObject>>& AttachedObjects = SelectedPlaceable->GetAttachedChildrenObjects();

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

void ModelOutlinerWindow::RenderAttachedOutline(AttachableObject* Attachment)
{
    const list<unique_ptr<AttachableObject>>& AttachedObjects = Attachment->GetAttachedChildrenObjects();

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

void ModelOutlinerWindow::SetWindowPosToCenter()
{
    PreviousWindowPos = GetWindowPos();
    ImVec2 Center = GetMainViewport()->GetCenter();
    SetWindowPos(Center, ImGuiCond_Appearing);
}

void ModelOutlinerWindow::ResetWindowPosToPrevious()
{
    SetWindowPos(PreviousWindowPos, ImGuiCond_Appearing);
}

void ModelOutlinerWindow::DoModalAddPlaceableObject()
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

void ModelOutlinerWindow::DoModalAddAttachableObject()
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


