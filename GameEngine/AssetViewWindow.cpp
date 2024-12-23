#include "AssetViewWindow.h"
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

AssetViewWindow::AssetViewWindow(const std::string& windowID, AssetManager* assetManager)
	: AWindow(windowID), m_assetManagerCached(assetManager)
{
    m_assetManagerCached->RegisterAssetLoadedHandler("AddAssetControl", bind(&AssetViewWindow::AddAssetControl, this, placeholders::_1, placeholders::_2, placeholders::_3));
}

void AssetViewWindow::InitThumbnailTextureAsAsset(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const EAssetThumbnailType& assetType, const UINT& width, const UINT& height, BaseTextureAsset* const asset)
{
    m_thumbnails[static_cast<size_t>(assetType)] = make_unique<Texture2DInstance<SRVOption>>(
        width, height, 1, asset->GetImageBuffers(), asset->GetRowPitchArray(), NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_B8G8R8X8_UNORM, device, deviceContext
    );
}

void AssetViewWindow::RenderWindowImpl()
{
    BeginGroup();
    PushID("RenderAssetFolder");

    ImVec2 RegionAvail = GetContentRegionAvail();
    
    BeginChild("AssetFolderStructure", ImVec2(RegionAvail.x * 0.2f, RegionAvail.y), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);
    RenderAssetFolderStructureByRecursive("Root", &m_assetRootFolders);
    EndChild();
    SameLine();
    BeginChild("SelectedAssetFolder", ImVec2(RegionAvail.x * 0.8f, RegionAvail.y), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);
    RenderSelectedAssetFolders(m_selectedFolders == nullptr ? &m_assetRootFolders : m_selectedFolders);
    EndChild();

    PopID();
    EndGroup();
}

void AssetViewWindow::InitializeWindow(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
    constexpr UINT thumbnailWidth = 128;
    constexpr UINT thumbnailHeight = 128;

    BaseTextureAsset* animationAssetTN = m_assetManagerCached->GetResourceAsset(AnimationAssetTN);
    BaseTextureAsset* boneAssetTN = m_assetManagerCached->GetResourceAsset(BoneAssetTN);
    BaseTextureAsset* textureAssetTN = m_assetManagerCached->GetResourceAsset(TextureAssetTN);
    BaseTextureAsset* skeletalMeshAssetTN = m_assetManagerCached->GetResourceAsset(SkeletalMeshAssetTN);
    BaseTextureAsset* staticMeshAssetTN = m_assetManagerCached->GetResourceAsset(StaticMeshAssetTN);
    BaseTextureAsset* mapAssetTN = m_assetManagerCached->GetResourceAsset(MapAssetTN);
    BaseTextureAsset* materialAssetTN = m_assetManagerCached->GetResourceAsset(MaterialAssetTN);

    InitThumbnailTextureAsAsset(device, deviceContext, EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_ANIMATION, thumbnailWidth, thumbnailHeight, animationAssetTN);
    InitThumbnailTextureAsAsset(device, deviceContext, EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_BONE, thumbnailWidth, thumbnailHeight, boneAssetTN);
    InitThumbnailTextureAsAsset(device, deviceContext, EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_TEXTURE, thumbnailWidth, thumbnailHeight, textureAssetTN);
    InitThumbnailTextureAsAsset(device, deviceContext, EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_STATIC, thumbnailWidth, thumbnailHeight, staticMeshAssetTN);
    InitThumbnailTextureAsAsset(device, deviceContext, EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_SKELETAL, thumbnailWidth, thumbnailHeight, skeletalMeshAssetTN);
    InitThumbnailTextureAsAsset(device, deviceContext, EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_MATERIAL, thumbnailWidth, thumbnailHeight, materialAssetTN);
    InitThumbnailTextureAsAsset(device, deviceContext, EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_MAP, thumbnailWidth, thumbnailHeight, mapAssetTN);
}

void AssetViewWindow::AddAssetControl(const EAssetType& assetType, const string& assetPath, AAsset* asset)
{
    AddAssetByRecursiveKey(assetType, m_assetRootFolders, assetPath, asset);
}

void AssetViewWindow::AddAssetByRecursiveKey(
    const EAssetType& assetType, SAssetFolder& targetFolder, const string& assetPath, AAsset* asset
)
{
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
            thumbnailSRV = m_thumbnails[static_cast<size_t>(EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_STATIC)]->GetSRV();
            break;
        case EAssetType::ASSET_TYPE_SKELETAL:
            thumbnailSRV = m_thumbnails[static_cast<size_t>(EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_SKELETAL)]->GetSRV();
            break;
        case EAssetType::ASSET_TYPE_BONE:
            thumbnailSRV = m_thumbnails[static_cast<size_t>(EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_BONE)]->GetSRV();
            break;
        case EAssetType::ASSET_TYPE_ANIMATION:
            thumbnailSRV = m_thumbnails[static_cast<size_t>(EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_ANIMATION)]->GetSRV();
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
            thumbnailSRV = m_thumbnails[static_cast<size_t>(EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_MATERIAL)]->GetSRV();
            break;
        case EAssetType::ASSET_TYPE_MAP:
            thumbnailSRV = m_thumbnails[static_cast<size_t>(EAssetThumbnailType::ASSET_THUMBNAIL_TYPE_MAP)]->GetSRV();
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

        ClearRegisteredControl();
        for (auto& assetControl : m_selectedFolders->m_assetControls)
        {
            RegisterControl(assetControl.get());
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
