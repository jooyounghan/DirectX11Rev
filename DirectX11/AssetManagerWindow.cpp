#include "AssetManagerWindow.h"
#include "AssetManager.h"
#include "GlobalVariable.h"

using namespace ImGui;
using namespace std;
using namespace filesystem;

ImVec2 AssetManagerWindow::FileSize = ImVec2(100, 100);

AssetManagerWindow::AssetManagerWindow(AssetManager* AssetManagerIn)
	: AssetManagerCached(AssetManagerIn)
{
    RootDirectory.Name = "Assets";
    RootDirectory.Directory = ".\\Assets\\";
    TravelAssetDirectories(RootDirectory);
}

AssetManagerWindow::~AssetManagerWindow()
{
}

void AssetManagerWindow::RenderWindow()
{

	Begin("Asset Manager");

    PushStyleColor(ImGuiCol_Header, Color::GDarkBrown);
    PushStyleColor(ImGuiCol_HeaderHovered, Color::GMediumBrown);
    PushStyleColor(ImGuiCol_HeaderActive, Color::GMediumBrown);
    PushStyleColor(ImGuiCol_Text, Color::GBlack);

    PushStyleColor(ImGuiCol_FrameBg, Color::GLightBrown);
    RenderAssetDirectoryStructure();
    PopStyleColor();

    SameLine();

    PopStyleColor();
    RenderCurrentDirectoryAsset();

    PopStyleColor(3);

    End();
}

void AssetManagerWindow::RenderAssetDirectoryStructure()
{
    ImVec2 RegionAvail = GetContentRegionAvail();
    BeginChild("Directories", ImVec2(RegionAvail.x * 0.2f, RegionAvail.y), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);
    RenderAssetDirectories(RootDirectory);
    EndChild();
}

void AssetManagerWindow::TravelAssetDirectories(DirectorySet& DirectorySetIn)
{
    path Directory = DirectorySetIn.Directory;
    if (!exists(Directory) && create_directories(Directory)) {/* Do Nothing But Make Directory */ };

    for (const auto& entry : directory_iterator(Directory))
    {
        if (entry.is_directory())
        {
            const path& EntryPath = entry.path();
            vector<DirectorySet>& ChildDirectories = DirectorySetIn.ChildrenDirectories;
            ChildDirectories.emplace_back(DirectorySet());

            DirectorySet& CurrentDirectory = ChildDirectories[ChildDirectories.size() - 1];
            CurrentDirectory.Name = EntryPath.filename().string();
            CurrentDirectory.Directory = EntryPath.string();
            TravelAssetDirectories(CurrentDirectory);
        }
    }
}

void AssetManagerWindow::RenderAssetDirectories(DirectorySet& DirectorySetIn)
{
    ImGuiTreeNodeFlags NodeFlags = BaseFlags;
    if (SelectedDirectory == &DirectorySetIn)
    {
        NodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    bool IsOpen = TreeNodeEx(DirectorySetIn.Name.c_str(), NodeFlags);

    if (ImGui::IsItemClicked() or ImGui::IsItemToggledOpen())
    {
        SelectedDirectory = &DirectorySetIn;
    }

    if (IsOpen)
    {     
        Indent();
        for (auto& ChildDirectory : DirectorySetIn.ChildrenDirectories)
        {
            RenderAssetDirectories(ChildDirectory);
        }
        Unindent();
        TreePop();
    }
}

void AssetManagerWindow::RefreshAssetDirectories()
{
    TravelAssetDirectories(RootDirectory);
}

void AssetManagerWindow::RenderCurrentDirectoryAsset()
{
    ImVec2 RegionAvail = GetContentRegionAvail();
    BeginChild("Files", RegionAvail, ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);
    if (SelectedDirectory != nullptr)
    {
        path CurrentDirectory = SelectedDirectory->Directory;
        

        float VisibleWidth = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        for (const auto& entry : directory_iterator(CurrentDirectory))
        {
            if (entry.is_regular_file())
            {
                const path& EntryPath = entry.path();
                RenderAssetFile(EntryPath, VisibleWidth);
            }
        }
    }
    EndChild();
}

void AssetManagerWindow::RenderAssetFile(const path& AssetPathIn, const float& VisibleWidthIn)
{
    ImGuiStyle& Style = ImGui::GetStyle();
    const path& EntryFileName = AssetPathIn.filename();

    ImGui::BeginGroup();

    ImGui::PushID(EntryFileName.c_str());
    ImGui::Button("", FileSize);
    ImGui::PopID();

    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + FileSize.x);
    ImGui::Text(EntryFileName.string().c_str());
    ImGui::PopTextWrapPos();

    ImGui::EndGroup();

    float LastFinishedWidthPos = ImGui::GetItemRectMax().x;
    float NextStartWidthPos = LastFinishedWidthPos + Style.ItemSpacing.x + ImGui::GetItemRectSize().x;
    if (NextStartWidthPos < VisibleWidthIn) ImGui::SameLine();
}

