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
    ID3D11Device* const* deviceAddress,
    ID3D11DeviceContext* const* immediateContextAddress,
    ID3D11DeviceContext* const* componentRenderDefferedContextAddress,
    AssetManager* assetManager,
    ComponentManager* componentManager,
    ComponentPSOManager* componentPsoManager
)
    : AWindow(windowID, true, nullptr),
    m_immediateContextAddress(immediateContextAddress),
    m_componentRenderDefferedContextAddress(componentRenderDefferedContextAddress),
    m_componentManagerCached(componentManager),
    m_componentPsoManageCached(componentPsoManager),
    m_componentInformer(assetManager, componentManager),
    m_forwardRenderer(componentRenderDefferedContextAddress, m_componentPsoManageCached, &m_selectedCamera, &m_selectedScene),
    m_defferedRenderer(deviceAddress, componentRenderDefferedContextAddress, m_componentPsoManageCached, &m_selectedCamera, &m_selectedScene),
    m_rendererComboBox("RendererComboBox", "", ImGuiComboFlags_WidthFitPreview)
{
    m_rendererComboBox.SetSelectableItems("Select Rederer", { "Forward Renderer", "Deffered Renderer" });
    m_rendererComboBox.OnSelChanged = [&](const size_t& idx, const string& text) 
        {
            ERendererType renderType = static_cast<ERendererType>(idx);
            switch (renderType)
            {
            case ERendererType::FORWARD_RENDERING:
                m_selectedRenderer = &m_forwardRenderer;
                m_isDefferedRenderer = false;
                break;
            case ERendererType::DEFFERED_RENDERING:
                m_selectedRenderer = &m_defferedRenderer;
                m_isDefferedRenderer = true;
                break;
            default:
                break;
            }
        };
}


void SceneWindow::PrepareWindow()
{
    try
    {

        if (m_selectedScene != nullptr && m_selectedRenderer != nullptr)
        {
            m_selectedRenderer->ClearRenderTargets();
            m_selectedScene->Accept(m_selectedRenderer);

            const vector<AComponent*>& sceneRootComponents = m_selectedScene->GetRootComponents();
            RenderComponentRecursive(m_selectedRenderer, sceneRootComponents);

            m_selectedRenderer->PostProcess();
        }
    }
    catch (std::exception& e)
    {
        printf(e.what());
    }
}


void SceneWindow::RenderComponentRecursive(ASceneRenderer* const renderer, const vector<AComponent*>& components)
{
    for (AComponent* component : components)
    {
        if (component->IsRenderable())
        {
            component->Accept(renderer);
            RenderComponentRecursive(renderer, component->GetChildComponents());
        }
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
    Text("Renderer Selector");
    m_rendererComboBox.Draw();
}

void SceneWindow::DrawScene()
{
    const ImVec2 cursorPos = GetCursorPos();
    const ImVec2 contentAvailRegion = GetContentRegionAvail();

    const Texture2DInstance<SRVOption, RTVOption, UAVOption>* const film = m_selectedCamera->GetFilm();
    Image((ImU64)(film != nullptr ? film->GetSRV() : nullptr), contentAvailRegion);

    if (m_isDefferedRenderer)
    {
        ImVec2 currentDebugImagePos = cursorPos;
        const vector<ID3D11ShaderResourceView*> gBufferSRVs = m_defferedRenderer.GetGBufferSRVs();
        const ImVec2 debugImageRect = ImVec2(contentAvailRegion.x / gBufferSRVs.size(), contentAvailRegion.y / 10.f);
        for (ID3D11ShaderResourceView* srv : gBufferSRVs)
        {
            SetCursorPos(currentDebugImagePos);
            SetNextItemAllowOverlap();
            Image((ImU64)(srv), debugImageRect);
            currentDebugImagePos.x += debugImageRect.x;
        }
    }


    SetCursorPos(cursorPos);
    InvisibleButton("SceneInteractor", contentAvailRegion);
    InteractSceneInput(contentAvailRegion);
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

    if (BeginPopup(contextMenuID.c_str()))
    {
        if (MenuItem("Add Component To Selected Comopnent"))
        {
            // Insert Window
        }
        if (MenuItem("Remove Selected Component"))
        {
            // Remove Data
        }
        EndPopup();
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

void SceneWindow::InteractSceneInput(const ImVec2& size)
{
    ImGuiIO& io = GetIO();

    if (m_selectedCamera != nullptr)
    {
        if (IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {
            uint32_t selectedComponentID = GetMouseClickedComponentID();
            m_selectedComponent = m_componentManagerCached->GetComponent(selectedComponentID);
        }

        if (IsItemHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_None))
        {
            if (io.MouseDown[ImGuiMouseButton_::ImGuiMouseButton_Middle])
            {
                const ImVec2 mouseDelta = io.MouseDelta;
                XMVECTOR& relativeAngle = const_cast<XMVECTOR&>(m_selectedCamera->GetLocalAngle());

                float& pitch = relativeAngle.m128_f32[0];
                float& yaw = relativeAngle.m128_f32[1];

                yaw += 360.f * (mouseDelta.x / size.x);
                pitch += 360.f * (mouseDelta.y / size.y);
                m_selectedCamera->SetIsModified(true);
            }
        }

        if (IsItemFocused())
        {
            XMVECTOR& relativePos = const_cast<XMVECTOR&>(m_selectedCamera->GetLocalPosition());
            const XMVECTOR rotationQuaternion = m_selectedCamera->GetLocalQuaternion();

            XMVECTOR currentForward = XMVector3Rotate(GDefaultForward, rotationQuaternion);
            XMVECTOR currentRight = XMVector3Rotate(GDefaultRight, rotationQuaternion);

            if (IsKeyDown(ImGuiKey::ImGuiKey_W))
            {
                relativePos += currentForward;
            }
            if (IsKeyDown(ImGuiKey::ImGuiKey_S))
            {
                relativePos -= currentForward;
            }
            if (IsKeyDown(ImGuiKey::ImGuiKey_A))
            {
                relativePos -= currentRight;
            }
            if (IsKeyDown(ImGuiKey::ImGuiKey_D))
            {
                relativePos += currentRight;
            }
            m_selectedCamera->SetIsModified(true);
        }
    }
}

uint32_t SceneWindow::GetMouseClickedComponentID()
{
    UINT IDResult = NULL;
    if (m_selectedCamera)
    {
        ImGuiIO& io = GetIO();
    
        ID3D11DeviceContext* immediateContext = (*m_immediateContextAddress);
        ID3D11Texture2D* idFilm = m_selectedCamera->GetIDFilm()->GetTexture2D();
        ID3D11Texture2D* idStagingFilm = m_selectedCamera->GetIDStatgingFilm()->GetTexture2D();

        const ImVec2 windowPos = GetWindowPos();
        const ImVec2 windowSize = GetWindowSize();
        const ImVec2& mousePos = io.MousePos;

        uint32_t posXOnWindow = static_cast<uint32_t>(((mousePos.x - windowPos.x) / windowSize.x) * m_selectedCamera->Width);
        uint32_t posYOnWindow = static_cast<uint32_t>(((mousePos.y - windowPos.y) / windowSize.y) * m_selectedCamera->Height);

        D3D11_BOX BoxData;
        AutoZeroMemory(BoxData);
        BoxData.left = posXOnWindow;
        BoxData.right = posXOnWindow + 1;
        BoxData.top = posYOnWindow;
        BoxData.bottom = posYOnWindow + 1;
        BoxData.front = 0;
        BoxData.back = 1;

        immediateContext->CopySubresourceRegion(idStagingFilm, 0, 0, 0, 0, idFilm, 0, &BoxData);

        D3D11_MAPPED_SUBRESOURCE idSelectedSubresource;
        AutoZeroMemory(idSelectedSubresource);
        immediateContext->Map(idStagingFilm, 0, D3D11_MAP_READ, NULL, &idSelectedSubresource);
        memcpy(&IDResult, idSelectedSubresource.pData, sizeof(uint32_t));
        immediateContext->Unmap(idStagingFilm, 0);
    }
    return IDResult;
}
