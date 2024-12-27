#include "SceneWindow.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;

SceneWindow::SceneWindow(
    const std::string& windowID,
    AssetManager* assetManager,
    ComponentManager* componentManager,
    PSOManager* psoManager
)
	: AWindow(windowID), m_componentManagerCached(componentManager), m_psoManageCached(psoManager), m_componentInformer(assetManager, componentManager)
{
}

void SceneWindow::RenderWindowImpl()
{
    BeginGroup();
    PushID("RenderScene");

    ImVec2 regionAvail = GetContentRegionAvail();
    BeginChild("SceneSelector", ImVec2(regionAvail.x * 0.1f, regionAvail.y));
    RenderSceneSelector();
    RenderRendererSelector();
    EndChild();
    SameLine();

    BeginChild("SceneViewport", ImVec2(regionAvail.x * 0.7f, regionAvail.y));
    RenderScene();
    EndChild();
    SameLine();

    BeginChild("ComponentHandler", ImVec2(regionAvail.x * 0.2f, regionAvail.y));
    RenderComponentTree();
    RenderComponentInformations();
    EndChild();

    PopID();
    EndGroup();
}

void SceneWindow::InitializeWindow(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
}

void SceneWindow::RenderSceneSelector()
{
    Text("Scene Selector");
    
    unordered_map<Scene*, string> scenesWithDescription = m_componentManagerCached->GetScenesWithDescription();
    ImVec2 regionAvail = GetContentRegionAvail();
    if (BeginListBox("SceneSelectListBox", ImVec2(regionAvail.x, regionAvail.y * 0.4f)))
    {
        for (auto& sceneWithDescription : scenesWithDescription)
        {
            const bool is_selected = (m_selectedScene == sceneWithDescription.first);
            if (Selectable(sceneWithDescription.second.c_str(), is_selected))
                m_selectedScene = sceneWithDescription.first;

            if (is_selected)
                SetItemDefaultFocus();
        }
        EndListBox();
    }
}

void SceneWindow::RenderRendererSelector()
{
    Text("Renderer Selector");
    Button("Renderer Selector", GetContentRegionAvail());
}

void SceneWindow::RenderScene()
{
    Image((ImU64)nullptr, GetContentRegionAvail());
}

void SceneWindow::RenderComponentTree()
{
    Text("Components");

    ImVec2 regionAvail = GetContentRegionAvail();
    BeginChild("RenderComponentTree", ImVec2(regionAvail.x, regionAvail.y * 0.6f));

    if (TreeNodeEx("Root"))
    {
        if (m_selectedScene != nullptr)
        {
            vector<AComponent*> rootComponents = m_selectedScene->GetRootComponents();
            for (AComponent* const rootComponent : rootComponents)
            {
                RenderComponentRecursive(rootComponent);
            }
        }
        TreePop();
    }

    EndChild();
}

void SceneWindow::RenderComponentInformations()
{
    Text("Informations");
    BeginChild("ComponentInformations", GetContentRegionAvail());
    if (m_selectedComponent != nullptr)
    {
        m_selectedComponent->Accept(&m_componentInformer);
    }
    EndChild();
}

void SceneWindow::RenderComponentRecursive(AComponent* const component)
{
    int nodeFlags = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnDoubleClick;
    vector<AComponent*> childComponents = component->GetChildComponents();

    if (childComponents.size() == 0)
    {
        nodeFlags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;
    }

    if (m_selectedComponent == component)
        nodeFlags |= ImGuiTreeNodeFlags_Selected;

    bool nodeOpen = TreeNodeEx(component->GetComponentName().c_str(), nodeFlags);

    std::string contextMenuID = format("{}ContextMenu", component->GetComponentName());
    if (IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
    {
        m_selectedComponent = component;
    }
    else if (IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
    {
        OpenPopup(contextMenuID.c_str());
    }

    if (ImGui::BeginPopup(contextMenuID.c_str()))
    {
        if (ImGui::MenuItem("Add Component To Selected Comopnent"))
        {
            // Insert Window
        }
        if (ImGui::MenuItem("Remove Selected Component"))
        {
            // Remove Data
        }
        ImGui::EndPopup();
    }

    if (nodeOpen)
    {
        for (AComponent* childComponent : childComponents)
        {
            RenderComponentRecursive(childComponent);
        }
        TreePop();
    }

}
