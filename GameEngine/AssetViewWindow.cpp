#include "AssetViewWindow.h"

#include "AssetManager.h"
#include "AssetFileControl.h"

#include "Texture2DInstance.h"
#include "SRVOption.h"

#include "AssetContextMenu.h"
#include "CreateModelMaterialWIthNodeEditorWindow.h"
#include "CreateIBLMaterialWithNodeEditorWindow.h"
#include "CreateAnimationRetargeterWindow.h"
#include <filesystem>

using namespace std;
using namespace ImGui;
using namespace filesystem;

string AssetViewWindow::AnimationAssetTN = "ANIMATIONASSETTN";
string AssetViewWindow::BoneAssetTN = "BONEASSETTN";
string AssetViewWindow::TextureAssetTN = "TEXTUREASSETTN";
string AssetViewWindow::SkeletalMeshAssetTN = "SKELETALMESHASSETTN";
string AssetViewWindow::StaticMeshAssetTN = "STATICMESHASSETTN";
string AssetViewWindow::MapAssetTN = "MAPASSETTN";
string AssetViewWindow::MaterialAssetTN = "MATERIALASSETTN";

AssetViewWindow::AssetViewWindow(
    const std::string& windowID, 
    ID3D11Device* const* deviceAddress,
    ID3D11DeviceContext* const* deviceContextAddress
)
	: AWindow(windowID, true, nullptr)
{
    AssetManager* assetManager = AssetManager::GetInstance();

    assetManager->RegisterAssetLoadedHandler("AddAssetControl", bind(&AssetViewWindow::AddAssetControl, this, placeholders::_1, placeholders::_2, placeholders::_3));
    
    m_assetContextMenuModal = new AssetContextMenu("Asset Context Menu");

    m_assetEditorWindows.emplace_back(new CreateModelMaterialWIthNodeEditorWindow(
        deviceAddress, deviceContextAddress, "Create Model Material Asset",
        m_assetContextMenuModal->GetCreateModelMaterialOpenFlag()
    ));

    m_assetEditorWindows.emplace_back(new CreateIBLMaterialWithNodeEditorWindow(
        deviceAddress, deviceContextAddress, "Create IBL Material Asset",
        m_assetContextMenuModal->GetCreateIBLMaterialOpenFlag()
    ));
    m_assetEditorWindows.emplace_back(new CreateAnimationRetargeterWindow(
        deviceAddress, deviceContextAddress,
        "Create IBL Material Asset", m_assetContextMenuModal->GetCreateRetargetedAnimationOpenFlag()
    ));
}

AssetViewWindow::~AssetViewWindow()
{
    for (AWindow* assetNodeEditorWindow : m_assetEditorWindows)
    {
        delete assetNodeEditorWindow;
    }
    delete m_assetContextMenuModal;
}

void AssetViewWindow::PrepareWindow()
{
    for (AWindow* assetNodeEditorWindow : m_assetEditorWindows)
    {
        assetNodeEditorWindow->PrepareWindow();
    }
}

void AssetViewWindow::RenderWindowImpl()
{
    m_assetContextMenuModal->DrawNotificator();

    BeginGroup();
    PushID("RenderAssetFolder");

    ImVec2 regionAvail = GetContentRegionAvail();
    BeginChild("AssetFolderStructure", ImVec2(regionAvail.x * 0.2f, regionAvail.y), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);
    RenderAssetFolderStructureByRecursive("Root", &m_assetRootFolders);
    EndChild();
    SameLine();

    BeginChild("SelectedAssetFolder", ImVec2(regionAvail.x * 0.8f, regionAvail.y), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);
    RenderSelectedAssetFolders(m_selectedFolders == nullptr ? &m_assetRootFolders : m_selectedFolders);
    EndChild();

    PopID();
    EndGroup();

    for (AWindow* assetNodeEditorWindow : m_assetEditorWindows)
    {
        assetNodeEditorWindow->ShowWindow();
    }

}

void AssetViewWindow::AddAssetControl(const EAssetType& assetType, const string& assetPath, AAsset* asset)
{
    AddAssetByRecursiveKey(assetType, m_assetRootFolders, assetPath, asset);
}

void AssetViewWindow::AddAssetByRecursiveKey(
    const EAssetType& assetType, SAssetFolder& targetFolder, const string& assetPath, AAsset* asset
)
{
    AssetManager* assetManager = AssetManager::GetInstance();

    std::string normalizedPath = assetPath;
    const path& _assetPath = assetPath;
    std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');

    std::string::size_type firstSlash = normalizedPath.find('/');
    if (firstSlash != std::string::npos) 
    {
        std::string firstComponent = normalizedPath.substr(0, firstSlash);
        std::string remainingPath = normalizedPath.substr(firstSlash + 1);

        AddAssetByRecursiveKey(assetType, targetFolder.m_childFolders[firstComponent], remainingPath, asset);
    }
    else
    {
        ID3D11ShaderResourceView* thumbnailSRV = nullptr;

        switch (assetType)
        {
        case EAssetType::ASSET_TYPE_STATIC:
            thumbnailSRV = assetManager->GetResourceAsset(StaticMeshAssetTN)->GetSRV();
            break;
        case EAssetType::ASSET_TYPE_SKELETAL:
            thumbnailSRV = assetManager->GetResourceAsset(SkeletalMeshAssetTN)->GetSRV();
            break;
        case EAssetType::ASSET_TYPE_BONE:
            thumbnailSRV = assetManager->GetResourceAsset(BoneAssetTN)->GetSRV();
            break;
        case EAssetType::ASSET_TYPE_ANIMATION:
            thumbnailSRV = assetManager->GetResourceAsset(AnimationAssetTN)->GetSRV();
            break;
        case EAssetType::ASSET_TYPE_BASE_TEXTURE:
        case EAssetType::ASSET_TYPE_SCRATCH_TEXTURE:
        {
            ATextureAsset* textureAsset = (ATextureAsset*)asset;
            thumbnailSRV = textureAsset->GetSRV();
            break;
        }
        case EAssetType::ASSET_TYPE_MODEL_MATERIAL:
        case EAssetType::ASSET_TYPE_IBL_MATERIAL:
            thumbnailSRV = assetManager->GetResourceAsset(MaterialAssetTN)->GetSRV();
            break;
        default:
            break;
        }

        targetFolder.m_assetControls.emplace_back(make_unique<AssetFileControl>(assetType, asset, thumbnailSRV));
    }
}

void AssetViewWindow::RenderAssetFolderStructureByRecursive(const string& folderName, const SAssetFolder* const assetFolder)
{
    ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth;
    if (assetFolder->m_childFolders.size() == 0)
    {
        NodeFlags |= ImGuiTreeNodeFlags_Leaf;
    }
    else
    {
        NodeFlags |= (ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow);
    }

    if (m_selectedFolders == assetFolder)
    {
        NodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    bool IsOpen = TreeNodeEx(folderName.c_str(), NodeFlags);

    if (ImGui::IsItemClicked() or ImGui::IsItemToggledOpen())
    {
        m_selectedFolders = assetFolder;

        ClearRegisteredInteractables();

        for (auto& assetControl : m_selectedFolders->m_assetControls)
        {
            assetControl->RegisterToInteractionManager(this);
        }
    }

    if (IsOpen)
    {
        Indent();
        for (const auto& childFolderWithName : assetFolder->m_childFolders)
        {
            const string& folderName = childFolderWithName.first;
            const SAssetFolder& assetFolder = childFolderWithName.second;
            RenderAssetFolderStructureByRecursive(folderName, &assetFolder);
        }
        Unindent();
        TreePop();
    }
}

void AssetViewWindow::RenderSelectedAssetFolders(const SAssetFolder* const assetFolder)
{

    float windowWidth = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

    const vector<unique_ptr<AssetFileControl>>& assetFileControls = assetFolder->m_assetControls;
    ImGuiStyle& Style = ImGui::GetStyle();

    for (auto& assetFileControl : assetFileControls)
    {
        assetFileControl->RenderControl();

        float lastXPos = ImGui::GetItemRectMax().x;
        float nextXPos = lastXPos + Style.ItemSpacing.x + ImGui::GetItemRectSize().x;
        if (nextXPos < windowWidth) ImGui::SameLine();
    }
}
