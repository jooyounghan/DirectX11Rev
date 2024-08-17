#include "ModelOutlinerWindow.h"

#include "EditorWorld.h"
#include "GameWorld.h"
#include "Map.h"

#include <list> 
#include <memory>

using namespace std;
using namespace ImGui;

const char* ModelOutlinerWindow::AddPlaceableButtonID = "Add Placeable +";
const char* ModelOutlinerWindow::AddAttachableButtonID = "Add Attacheable +";
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
    ImGui::Begin("Map Outliner");
    RenderPlaceablesOutline();
    RenderSelectedPlaceableOutline();
    ImGui::End();
}

void ModelOutlinerWindow::RenderPlaceablesOutline()
{
    RegionAvail = GetContentRegionAvail();
    BeginChild("Placeables Outliner", ImVec2(RegionAvail.x, RegionAvail.y * 0.5f));

    Map* CurrentMap = GameWorldCached->GetCurrentMap();
    if (CurrentMap)
    {
        ImGui::Text("Placed Object List:");
        SameLine();

        float ButtonWidth = CalcTextSize(AddPlaceableButtonID).x;
        SetCursorPosX(GetCursorPosX() + GetContentRegionAvail().x - ButtonWidth);
        if (SmallButton(AddPlaceableButtonID))
        {
            ImGui::OpenPopup(AddPlaceableModalID);
        }
        DoModalAddPlaceableObject();
        RenderPlacedListBox(CurrentMap);
        IfDeletePlaceableObject();
    }
    EndChild();
}

void ModelOutlinerWindow::RenderSelectedPlaceableOutline()
{
    Text("Attached Object List:\t");
    SameLine();

    float ButtonWidth = CalcTextSize(AddAttachableButtonID).x;
    SetCursorPosX(GetCursorPosX() + GetContentRegionAvail().x - ButtonWidth);
    if (SmallButton(AddAttachableButtonID))
    {
        ImGui::OpenPopup(AddAttachableModalID);
    }
    DoModalAddAttachableObject();
    RenderAttachedTree();
    IfDeleteAttachedObject();
}

void ModelOutlinerWindow::IfDeletePlaceableObject()
{
    if (IsKeyPressed(ImGuiKey::ImGuiKey_Delete) && IsItemFocused())
    {
        // Delete Selected Placeable Object
    }
}
void ModelOutlinerWindow::IfDeleteAttachedObject()
{
    if (IsKeyPressed(ImGuiKey::ImGuiKey_Delete) && IsItemFocused())
    {
        // Delete Selected Attached Object
    }
}
void ModelOutlinerWindow::RenderPlacedListBox(Map* CurrentMap)
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
        if (ImGui::IsItemClicked() or ImGui::IsItemToggledOpen())
        {
            EditorWorldCached->SetSelectedPlaceable(PlaceableObj);
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

        if (SelectedPlaceable == EditorWorldCached->GetSelectedDetailed())
        {
            NodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        bool IsOpen = TreeNodeEx(SelectedPlaceable->GetObjectName().c_str(), NodeFlags);
        if (ImGui::IsItemClicked() or ImGui::IsItemToggledOpen())
        {
            EditorWorldCached->SetSelectedDetailed(SelectedPlaceable);
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

    if (EditorWorldCached->GetSelectedDetailed() == Attachment)
    {
        NodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    bool IsOpen = TreeNodeEx(Attachment->GetObjectName().c_str(), NodeFlags);
    if (ImGui::IsItemClicked() or ImGui::IsItemToggledOpen())
    {
        EditorWorldCached->SetSelectedDetailed(Attachment);
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

void ModelOutlinerWindow::DoModalAddPlaceableObject()
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal(AddPlaceableModalID, NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Test1");
        ImGui::Separator();
        ImGui::EndPopup();
    }
}

void ModelOutlinerWindow::DoModalAddAttachableObject()
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal(AddAttachableModalID, NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Test2");
        ImGui::Separator();
        ImGui::EndPopup();
    }
}


