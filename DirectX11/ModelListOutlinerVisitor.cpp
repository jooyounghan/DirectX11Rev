#include "ModelListOutlinerVisitor.h"
#include "MapOutlinerWindow.h"

#include "EditorWorld.h"
#include "AttachableObject.h"
#include "PlaceableObject.h"
#include "RelativePlaceableObject.h"

#include <list>
#include <memory>
#include <format>

using namespace std;
using namespace ImGui;

ModelListOutlinerVisitor::ModelListOutlinerVisitor(MapOutlinerWindow* MapOutlinerInstance)
    : MapOutlinerCached(MapOutlinerInstance)
{
    EditorWorldCached = MapOutlinerCached->GetEditorWorldCached();
    assert(EditorWorldCached != nullptr);
}

void ModelListOutlinerVisitor::Visit(AttachableObject* AttachableInstance)
{
    return DrawAttachableInOutliner(AttachableInstance);
}

void ModelListOutlinerVisitor::Visit(PlaceableObject* PlaceableInstance)
{
    return DrawPlaceableInOutliner(PlaceableInstance);
}

void ModelListOutlinerVisitor::Visit(RelativePlaceableObject* RelativePlaceableObject)
{
    return DrawPlaceableInOutliner(RelativePlaceableObject);
}

void ModelListOutlinerVisitor::DrawAttachableInOutliner(AAttachable* Attachable)
{
    AObject* ObjectedInstance = (AObject*)Attachable;
    ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (EditorWorldCached->GetSelectedObject() == ObjectedInstance)
    {
        NodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    const string AttachTreeId{ std::format("{}", (uint64_t)Attachable) };
    PushID(AttachTreeId.c_str());
    bool IsOpen = TreeNodeEx(ObjectedInstance->GetObjectName().c_str(), NodeFlags);
    PopID();
    if (ImGui::IsItemClicked() or ImGui::IsItemToggledOpen())
    {
        EditorWorldCached->SetSelectedObject(ObjectedInstance);
    }
}

void ModelListOutlinerVisitor::DrawPlaceableInOutliner(APlaceable* Placeable)
{
    AObject* ObjectedInstance = (AObject*)Placeable;
    const list<unique_ptr<AAttachable>>& AttachedObjects = Placeable->GetAttachedObjects();

    ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth;
    if (AttachedObjects.size() == 0)
    {
        NodeFlags |= ImGuiTreeNodeFlags_Leaf;
    }
    else
    {
        NodeFlags |= (ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow);
    }

    if (EditorWorldCached->GetSelectedObject() == ObjectedInstance)
    {
        NodeFlags |= ImGuiTreeNodeFlags_Selected;
    }
    const string AttachTreeId{ std::format("{}", (uint64_t)Placeable) };
    PushID(AttachTreeId.c_str());
    bool IsOpen = TreeNodeEx(ObjectedInstance->GetObjectName().c_str(), NodeFlags);
    PopID();
    if (ImGui::IsItemClicked() or ImGui::IsItemToggledOpen())
    {
        EditorWorldCached->SetSelectedObject(ObjectedInstance);
    }

    if (IsOpen)
    {
        Indent();
        for (auto& AttachedObject : AttachedObjects)
        {
            AttachedObject->AcceptGui(this);
        }
        Unindent();
        TreePop();
    }
}
