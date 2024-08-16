#include "ModelOutlinerWindow.h"

#include "EditorWorld.h"
#include "GameWorld.h"
#include "Map.h"

#include <list> 
#include <memory>

using namespace std;
using namespace ImGui;

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
    BeginChild("Placeables Outliner", ImVec2(RegionAvail.x, RegionAvail.y * 0.5f), ImGuiChildFlags_FrameStyle);
    Map* CurrentMap = GameWorldCached->GetCurrentMap();
    if (CurrentMap)
    {
        ImGui::Text("Placed Object List:");
        if (ImGui::BeginListBox("PlacedListBox", ImVec2(-FLT_MIN, RegionAvail.y * 0.5f)))
        {
            const list<unique_ptr<PlaceableObject>>& RootPlaceables = CurrentMap->GetRootPlaceables();
            for (auto& Placeable : RootPlaceables)
            {
                PlaceableObject* PlaceableObj = Placeable.get();
                const bool is_selected = (EditorWorldCached->GetSelectedPlaceable() == PlaceableObj);
                if (Selectable(Placeable->GetObjectName().c_str(), is_selected))
                {
                    EditorWorldCached->SetSelectedPlaceable(PlaceableObj);
                }
                if (is_selected)
                {
                    SetItemDefaultFocus();
                }
            }
            EndListBox();
        }
    }
    EndChild();
}

void ModelOutlinerWindow::RenderSelectedPlaceableOutline()
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


