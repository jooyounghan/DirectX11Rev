#include "SceneWindow.h"

#include "Scene.h"
#include "CameraComponent.h"
#include "ComponentManager.h"

#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "RTVOption.h"
#include "UAVOption.h"
#include "DSVOption.h"

#include "LightManager.h"
#include "SpotLightComponent.h"
#include "PointLightComponent.h"

#include "RenderControlOption.h"

#include "PerformanceAnalyzer.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;

SceneWindow::SceneWindow(
    const string& windowID,
    ID3D11Device* device,
    ID3D11DeviceContext* immediateContext,
    ID3D11DeviceContext* componentRenderDeferredContext,
    ComponentManager* componentManager,
    ComponentPSOManager* componentPsoManager
)
    : AWindow(windowID, true, nullptr),
    m_immediateContext(immediateContext),
    m_componentRenderDeferredContext(componentRenderDeferredContext),
    m_componentManagerCached(componentManager),
    m_componentPsoManageCached(componentPsoManager),
    m_componentHandler(componentManager),
    m_forwardRenderer(componentRenderDeferredContext, m_componentPsoManageCached, &m_selectedCamera, &m_selectedScene),
    m_deferredRenderer(device, componentRenderDeferredContext, m_componentPsoManageCached, &m_selectedCamera, &m_selectedScene),
    m_rendererComboBox("RendererComboBox", "", ImGuiComboFlags_WidthFitPreview)
{
    m_rendererComboBox.SetSelectableItems("Select Rederer", { "Forward Renderer", "Deferred Renderer" });
    m_rendererComboBox.OnSelChanged = [&](const size_t& idx, const string& text) 
        {
            ERendererType renderType = static_cast<ERendererType>(idx);
            switch (renderType)
            {
            case ERendererType::FORWARD_RENDERING:
                m_selectedRenderer = &m_forwardRenderer;
                m_isDeferredRenderer = false;
                break;
            case ERendererType::Deferred_RENDERING:
                m_selectedRenderer = &m_deferredRenderer;
                m_isDeferredRenderer = true;
                break;
            default:
                break;
            }
        };
}


void SceneWindow::PrepareWindow()
{
    if (m_selectedScene != nullptr && m_selectedRenderer != nullptr)
    {
        const uint32_t& sceneID = m_selectedScene->GetSceneID();

        m_selectedRenderer->ClearRenderTargets();
        m_selectedScene->Accept(m_selectedRenderer);

        LightManager& lightManager = m_selectedScene->GetLightManager();
        const vector<SpotLightComponent*>& spotLights = lightManager.GetSpotLights();
        const vector<PointLightComponent*>& pointLights = lightManager.GetPointLights();

        const vector<AComponent*>& sceneRootComponents = m_selectedScene->GetRootComponents();

        BoundingVolumeHierarchy& renderControlBVH = RenderControlOption::RenderBVHs[sceneID];

        renderControlBVH.ResetSerachCount();
        for (SpotLightComponent* spotLight : spotLights)
        {
            spotLight->GenerateShadowMap(
                m_componentRenderDeferredContext, 
                m_componentPsoManageCached, 
                sceneRootComponents
            );
        }

        for (PointLightComponent* pointLight : pointLights)
        {
            pointLight->GenerateShadowMap(
                m_componentRenderDeferredContext,
                m_componentPsoManageCached,
                sceneRootComponents
            );
        }
        PerformanceAnalyzer::DepthTestCollisionCheckCount = renderControlBVH.GetSerachCount();


        renderControlBVH.ResetSerachCount();
        const vector<AComponent*> renderableComponts = RenderControlOption::GetRenderableComponents(sceneID, m_selectedCamera, sceneRootComponents);
        for (AComponent* renderableCompont : renderableComponts)
        {
            renderableCompont->Accept(m_selectedRenderer);
        }
        PerformanceAnalyzer::RenderingCollisionCheckCount = renderControlBVH.GetSerachCount();

        m_selectedRenderer->PostProcess();
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

    Texture2DInstance<SRVOption, RTVOption, UAVOption>& film = m_selectedCamera->GetFilm();
    Image((ImU64)(film.GetSRV()), contentAvailRegion);

    if (m_isDeferredRenderer)
    {
        ImVec2 currentDebugImagePos = cursorPos;
        const vector<ID3D11ShaderResourceView*> gBufferSRVs = m_deferredRenderer.GetGBufferSRVs();
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
        m_selectedComponent->Accept(&m_componentHandler);
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
                XMVECTOR relativeAngle = m_selectedCamera->GetLocalAngle();

                float& pitch = relativeAngle.m128_f32[0];
                float& yaw = relativeAngle.m128_f32[1];

                yaw += 360.f * (mouseDelta.x / size.x);
                pitch += 360.f * (mouseDelta.y / size.y);

                m_selectedCamera->SetLocalAngle(relativeAngle);
            }
        }

        bool isChanged = false;
        if (IsItemFocused())
        {
            XMVECTOR relativePos = m_selectedCamera->GetLocalPosition();
            XMVECTOR rotationQuaternion = m_selectedCamera->GetLocalRotationQuaternion();

            XMVECTOR currentForward = XMVector3Rotate(GDefaultForward, rotationQuaternion);
            XMVECTOR currentRight = XMVector3Rotate(GDefaultRight, rotationQuaternion);

            if (IsKeyDown(ImGuiKey::ImGuiKey_W))
            {
                relativePos += currentForward;
                isChanged = true;
            }
            if (IsKeyDown(ImGuiKey::ImGuiKey_S))
            {
                relativePos -= currentForward;
                isChanged = true;
            }
            if (IsKeyDown(ImGuiKey::ImGuiKey_A))
            {
                relativePos -= currentRight;
                isChanged = true;
            }
            if (IsKeyDown(ImGuiKey::ImGuiKey_D))
            {
                relativePos += currentRight;
                isChanged = true;
            }

            if (isChanged)
            {
                m_selectedCamera->SetLocalPosition(relativePos);
            }
        }


    }
}

uint32_t SceneWindow::GetMouseClickedComponentID()
{
    UINT IDResult = NULL;
    if (m_selectedCamera)
    {
        ImGuiIO& io = GetIO();
    
        ID3D11Texture2D* idFilm = m_selectedCamera->GetIDFilm().GetTexture2D();
        ID3D11Texture2D* idStagingFilm = m_selectedCamera->GetIDStatgingFilm().GetTexture2D();

        const ImVec2 windowPos = GetWindowPos();
        const ImVec2 windowSize = GetWindowSize();
        const ImVec2& mousePos = io.MousePos;

        const D3D11_VIEWPORT& viewport = m_selectedCamera->GetViewport();
        uint32_t posXOnWindow = static_cast<uint32_t>(((mousePos.x - windowPos.x) / windowSize.x) * viewport.Width);
        uint32_t posYOnWindow = static_cast<uint32_t>(((mousePos.y - windowPos.y) / windowSize.y) * viewport.Height);

        D3D11_BOX BoxData;
        AutoZeroMemory(BoxData);
        BoxData.left = posXOnWindow;
        BoxData.right = posXOnWindow + 1;
        BoxData.top = posYOnWindow;
        BoxData.bottom = posYOnWindow + 1;
        BoxData.front = 0;
        BoxData.back = 1;

        m_immediateContext->CopySubresourceRegion(idStagingFilm, 0, 0, 0, 0, idFilm, 0, &BoxData);

        D3D11_MAPPED_SUBRESOURCE idSelectedSubresource;
        AutoZeroMemory(idSelectedSubresource);
        m_immediateContext->Map(idStagingFilm, 0, D3D11_MAP_READ, NULL, &idSelectedSubresource);
        memcpy(&IDResult, idSelectedSubresource.pData, sizeof(uint32_t));
        m_immediateContext->Unmap(idStagingFilm, 0);
    }
    return IDResult;
}
