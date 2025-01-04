#include "SceneWindow.h"

#include "Scene.h"
#include "CameraComponent.h"

#include "AssetManager.h"
#include "ComponentManager.h"
#include "ComponentPSOManager.h"

#include "DynamicBuffer.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;

SceneWindow::SceneWindow(
    const string& windowID,
    ID3D11DeviceContext** deviceConextAddress,
    AssetManager* assetManager,
    ComponentManager* componentManager,
    ComponentPSOManager* componentPsoManager
)
    : AWindow(windowID, true, nullptr), m_deviceContextAddressCached(deviceConextAddress),
    m_componentManagerCached(componentManager),
    m_componentPsoManageCached(componentPsoManager),
    m_componentInformer(assetManager, componentManager),
    m_forwardRenderer(deviceConextAddress, m_componentPsoManageCached),
    m_defferedRenderer(deviceConextAddress, m_componentPsoManageCached),
    m_rendererComboBox({ "Forward Renderer", "Deffered Renderer" }, "RendererComboBox", "", ImGuiComboFlags_WidthFitPreview)
{
    m_rendererComboBox.OnSelChanged = [&](const size_t& idx, const string& text) {m_selectedRendererType = static_cast<ERendererType>(idx); };
}

void SceneWindow::PrepareWindow()
{
    if (m_selectedScene != nullptr && m_selectedCamera != nullptr)
    {
        ASceneRenderer* sceneRenderer = nullptr;
        switch (m_selectedRendererType)
        {
        case ERendererType::FORWARD_RENDERING:
            sceneRenderer = &m_forwardRenderer;
            break;
        case ERendererType::DEFFERED_RENDERING:
            sceneRenderer = &m_defferedRenderer;
            break;
        default:
            throw invalid_argument("Invalid entity type");
        }

        RenderScene();

        const vector<AComponent*>& sceneRootComponents = m_selectedScene->GetRootComponents();
        RenderComponentRecursive(sceneRenderer, sceneRootComponents);
    }
}

void SceneWindow::RenderScene()
{
    ID3D11DeviceContext* deviceContext = *m_deviceContextAddressCached;
    
    AShader* sceneVertexShader = m_componentPsoManageCached->GetComponentPSOVertexShader(ComponentPSOManager::EComponentPSOVertexShader::SCENE);
    AShader* scenePixelShader = m_componentPsoManageCached->GetComponentPSOPixelShader(ComponentPSOManager::EComponentPSOPixelShader::FORWARD_SCENE);
    ID3D11SamplerState* wrapSamplerState = m_componentPsoManageCached->GetComponentPSOSamplerState(ComponentPSOManager::EComponentPSOSamplerState::WRAP);

    const Texture2DInstance<SRVOption, RTVOption, UAVOption>* const film = m_selectedCamera->GetFilm();
    
    const StaticMeshAsset* const staticMeshAsset = m_selectedScene->GetSceneMeshAsset();
    MeshPartsData* meshPartsData = staticMeshAsset->GetMeshPartData(NULL);

    const IBLMaterialAsset* const iblMaterialAsset = m_selectedScene->GetIBLMaterialAsset();
    const ScratchTextureAsset* const backgroundTextureAsset = iblMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_BACKGROUND);

    const size_t& meshPartCount = meshPartsData->GetPartsCount();
    const vector<UINT>& indicesOffsets = meshPartsData->GetIndexOffsets();
    const vector<ID3D11Buffer*> vertexBuffers = meshPartsData->GetVertexBuffers();
    const vector<UINT>& strides = meshPartsData->GetStrides();
    const vector<UINT>& verticesOffsets = meshPartsData->GetVertexOffsets();
    const UINT& totalIndicesCount = static_cast<UINT>(meshPartsData->GetIndices().size());

    for (size_t idx = 0; idx < meshPartCount; ++idx)
    {
        const UINT indexCount = (idx + 1 == meshPartCount ? totalIndicesCount : indicesOffsets[idx + 1]) - indicesOffsets[idx];

        sceneVertexShader->SetShader(deviceContext);
        deviceContext->IASetVertexBuffers(0, static_cast<UINT>(vertexBuffers.size()), vertexBuffers.data(), strides.data(), verticesOffsets.data());
        deviceContext->IASetIndexBuffer(meshPartsData->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, indicesOffsets[idx]);

        vector<ID3D11Buffer*> vsConstantBuffers{ m_selectedCamera->GetViewProjMatrixBuffer()->GetBuffer() };
        vector<ID3D11Buffer*> psConstantBuffers{ iblMaterialAsset->GetIBLToneMappingBuffer()->GetBuffer() };
        vector<ID3D11ShaderResourceView*> psSRVs{ backgroundTextureAsset->GetSRV() };

        deviceContext->VSSetConstantBuffers(0, 1, vsConstantBuffers.data());
        deviceContext->PSSetConstantBuffers(0, 1, psConstantBuffers.data());
        deviceContext->PSSetShaderResources(0, 1, psSRVs.data());

        deviceContext->DrawIndexed(indexCount, NULL, NULL);
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
    Image((ImU64)nullptr, GetContentRegionAvail());
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
