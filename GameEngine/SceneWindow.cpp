#include "SceneWindow.h"

#include "Scene.h"
#include "CameraComponent.h"
#include "ComponentManager.h"

#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "RTVOption.h"
#include "UAVOption.h"
#include "DSVOption.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;

SceneWindow::SceneWindow(
    const string& windowID,
    ID3D11DeviceContext* const* deviceConextAddress,
    AssetManager* assetManager,
    ComponentManager* componentManager,
    ComponentPSOManager* componentPsoManager
)
    : AWindow(windowID, true, nullptr),
    m_deviceContextAddressCached(deviceConextAddress),
    m_componentManagerCached(componentManager),
    m_componentPsoManageCached(componentPsoManager),
    m_componentInformer(assetManager, componentManager),
    m_forwardRenderer(deviceConextAddress, m_componentPsoManageCached, &m_selectedCamera),
    m_defferedRenderer(deviceConextAddress, m_componentPsoManageCached, &m_selectedCamera),
    m_rendererComboBox("RendererComboBox", "", ImGuiComboFlags_WidthFitPreview)
{
    m_rendererComboBox.SetSelectableItems({ "Forward Renderer", "Deffered Renderer" });
    m_rendererComboBox.OnSelChanged = [&](const size_t& idx, const string& text) 
        {
            ERendererType renderType = static_cast<ERendererType>(idx);
            switch (renderType)
            {
            case ERendererType::FORWARD_RENDERING:
                m_selectedRenderer = &m_forwardRenderer;
                break;
            case ERendererType::DEFFERED_RENDERING:
                m_selectedRenderer = &m_defferedRenderer;
                break;
            default:
                break;
            }
        };
}


void SceneWindow::PrepareWindow()
{
    if (m_selectedScene != nullptr && m_selectedCamera != nullptr && m_selectedRenderer != nullptr)
    {
        m_selectedScene->Accept(m_selectedRenderer);

        const vector<AComponent*>& sceneRootComponents = m_selectedScene->GetRootComponents();
        RenderComponentRecursive(m_selectedRenderer, sceneRootComponents);
    }
}


void SceneWindow::RenderComponentRecursive(ASceneRenderer* const renderer, const vector<AComponent*>& components)
{
    for (AComponent* component : components)
    {
        component->Accept(renderer);
        RenderComponentRecursive(renderer, component->GetChildComponents());
    }
}

void SceneWindow::RenderWindowImpl()
{
    ImVec2 regionAvail = GetContentRegionAvail();
    BeginChild("SceneSelector", ImVec2(regionAvail.x * 0.1f, regionAvail.y));
    DrawSceneSelector();
    DrawRendererSelector();
    EndChild();
    SameLine();

    BeginChild("SceneViewport", ImVec2(regionAvail.x * 0.7f, regionAvail.y));
    DrawScene();
    EndChild();
    SameLine();

    BeginChild("ComponentHandler", ImVec2(regionAvail.x * 0.2f, regionAvail.y));
    DrawComponentTree();
    DrawComponentInformations();
    EndChild();
}

void SceneWindow::DrawSceneSelector()
{
    Text("Scene Selector");
    
    const unordered_map<Scene*, string>& scenesWithDescription = m_componentManagerCached->GetScenesWithDescription();
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

void SceneWindow::DrawRendererSelector()
{
    static vector<string> rendererItems{ "Forward Rendering", "Deffered Rendering" };

    Text("Renderer Selector");
    m_rendererComboBox.Draw();
}

void SceneWindow::DrawScene()
{
    const Texture2DInstance<SRVOption, RTVOption, UAVOption>* const film = m_selectedCamera->GetFilm();
    Image((ImU64)(film != nullptr ? film->GetSRV() : nullptr), GetContentRegionAvail());
}

void SceneWindow::DrawComponentTree()
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
                DrawComponentTreeRecursive(rootComponent);
            }
        }
        TreePop();
    }

    EndChild();
}

void SceneWindow::DrawComponentInformations()
{
    Text("Informations");
    BeginChild("ComponentInformations", GetContentRegionAvail());
    if (m_selectedComponent != nullptr)
    {
        m_selectedComponent->Accept(&m_componentInformer);
    }
    EndChild();
}

void SceneWindow::DrawComponentTreeRecursive(AComponent* const component)
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

    string contextMenuID = format("{}ContextMenu", component->GetComponentName());
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
            DrawComponentTreeRecursive(childComponent);
        }
        TreePop();
    }

}
