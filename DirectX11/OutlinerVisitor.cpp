#include "OutlinerVisitor.h"
#include "MapOutlinerWindow.h"

#include "AttachableObject.h"
#include "PlaceableObject.h"
#include "RelativePlaceableObject.h"

#include <list>
#include <memory>
#include <format>

using namespace std;
using namespace ImGui;

OutlinerVisitor::OutlinerVisitor(MapOutlinerWindow* MapOutlinerInstance)
    : MapOutlinerCached(MapOutlinerInstance)
{
}

void OutlinerVisitor::Visit(AttachableObject* AttachableInstance)
{
    return RenderAttachableInOutliner(AttachableInstance);
}

void OutlinerVisitor::Visit(PlaceableObject* PlaceableInstance)
{
    return RenderPlaceableInOutliner(PlaceableInstance);
}

void OutlinerVisitor::Visit(RelativePlaceableObject* RelativePlaceableObject)
{
    return RenderPlaceableInOutliner(RelativePlaceableObject);
}

void OutlinerVisitor::RenderAttachableInOutliner(AAttachable* Attachable)
{
    if (MapOutlinerCached)
    {
        AObject* ObjectedInstance = (AObject*)Attachable;
        ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (MapOutlinerCached->GetSelectedObject() == ObjectedInstance)
        {
            NodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        const string AttachTreeId{ std::format("{}", (uint64_t)Attachable) };
        PushID(AttachTreeId.c_str());
        bool IsOpen = TreeNodeEx(ObjectedInstance->GetObjectName().c_str(), NodeFlags);
        PopID();
        if (ImGui::IsItemClicked() or ImGui::IsItemToggledOpen())
        {
            MapOutlinerCached->SetSelectedObject(ObjectedInstance);
        }
    }
}

void OutlinerVisitor::RenderPlaceableInOutliner(APlaceable* Placeable)
{
    if (MapOutlinerCached)
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

        if (MapOutlinerCached->GetSelectedObject() == ObjectedInstance)
        {
            NodeFlags |= ImGuiTreeNodeFlags_Selected;
        }
        const string AttachTreeId{ std::format("{}", (uint64_t)Placeable) };
        PushID(AttachTreeId.c_str());
        bool IsOpen = TreeNodeEx(ObjectedInstance->GetObjectName().c_str(), NodeFlags);
        PopID();
        if (ImGui::IsItemClicked() or ImGui::IsItemToggledOpen())
        {
            MapOutlinerCached->SetSelectedObject(ObjectedInstance);
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
}
