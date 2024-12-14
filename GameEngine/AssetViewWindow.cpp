#include "AssetViewWindow.h"
#include <filesystem>

using namespace std;
using namespace ImGui;
using namespace filesystem;

AssetViewWindow::AssetViewWindow(const std::string& windowID, AssetManager* assetManager)
	: AWindow(windowID), m_assetManagerCached(assetManager)
{
    m_assetManagerCached->OnAssetLoaded = bind(&AssetViewWindow::AddAssetControl, this, placeholders::_1, placeholders::_2);
}

void AssetViewWindow::RenderWindowImpl()
{
    BeginGroup();
    PushID("RenderAssetFolder");

    ImVec2 RegionAvail = GetContentRegionAvail();
    
    BeginChild("AssetFolderStructure", ImVec2(RegionAvail.x * 0.2f, RegionAvail.y), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);
    RenderAssetFolderStructureByRecursive("Root", &m_assetRootFolders);
    EndChild();
   
    BeginChild("SelectedAssetFolder");

    EndChild();

    PopID();
    EndGroup();
}

void AssetViewWindow::AddAssetControl(const string& assetPath, AAsset* asset)
{
    AddAssetByRecursiveKey(m_assetRootFolders, assetPath, asset);
}

void AssetViewWindow::AddAssetByRecursiveKey(
    SAssetFolder& targetFolder, const string& assetPath, AAsset* asset
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

        AddAssetByRecursiveKey(targetFolder.childFolders[firstComponent], remainingPath, asset);
    }
    else
    {
        targetFolder.assetControls.emplace_back(/*asset*/);
    }
}

void AssetViewWindow::RenderAssetFolderStructureByRecursive(const string& folderName, const SAssetFolder* const assetFolder)
{
    ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth;
    if (assetFolder->childFolders.size() == 0)
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
    }

    if (IsOpen)
    {
        Indent();
        for (const auto& childFolderWithName : assetFolder->childFolders)
        {
            const string& folderName = childFolderWithName.first;
            const SAssetFolder& assetFolder = childFolderWithName.second;
            RenderAssetFolderStructureByRecursive(folderName, &assetFolder);
        }
        Unindent();
        TreePop();
    }
}
