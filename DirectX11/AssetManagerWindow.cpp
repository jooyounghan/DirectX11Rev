#include "AssetManagerWindow.h"
#include "AssetManager.h"
#include "GlobalVariable.h"
#include "AAssetFile.h"

using namespace ImGui;
using namespace std;
using namespace filesystem;

AssetManagerWindow::AssetManagerWindow(AssetManager* AssetManagerIn)
	: AssetManagerCached(AssetManagerIn)
{
    RootDirectory.Name = "Assets";
    RootDirectory.Directory = ".\\Assets\\";

#pragma region Binding
    OnAssetChanged = bind(&AssetManagerWindow::RefreshAssetDirectories, this);
    AssetManagerCached->AssetChangedEvent += OnAssetChanged;

    OnAssetControlBeginDragDrop = bind(&AssetManagerWindow::SetAssetControlDragDrop, this, placeholders::_1);
    OnAssetControlPushHilightStyle = bind(&AssetManagerWindow::HilightItem, this, placeholders::_1);
    OnAssetControlPopHilightStyle = bind(&AssetManagerWindow::UnhilightItem, this, placeholders::_1);
#pragma endregion 

    RefreshAssetDirectories();
}

AssetManagerWindow::~AssetManagerWindow()
{
}

void AssetManagerWindow::RenderWindow()
{
	Begin("Asset Manager");

    RenderAssetDirectoryStructure();
    SameLine();
    RenderCurrentDirectoryAsset();
    End();
}

void AssetManagerWindow::RenderAssetDirectoryStructure()
{
    ImVec2 RegionAvail = GetContentRegionAvail();
    BeginChild("Directories", ImVec2(RegionAvail.x * 0.2f, RegionAvail.y), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);
    RenderAssetDirectories(RootDirectory);
    EndChild();
}

void AssetManagerWindow::RenderAssetDirectories(DirectorySet& DirectorySetIn)
{
    ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth;
    if (DirectorySetIn.ChildrenDirectories.size() == 0)
    {
        NodeFlags |= ImGuiTreeNodeFlags_Leaf;
    }
    else
    {
        NodeFlags |= (ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow);
    }

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

void AssetManagerWindow::RefreshAssetDirectories()
{
    SelectedDirectory = nullptr;
    RootDirectory.ChildrenDirectories.clear();
    TravelAssetDirectories(RootDirectory);
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

void AssetManagerWindow::SetAssetControlDragDrop(AAssetFile* AssetFileCached)
{
    const string& AssetName = AssetFileCached->GetAssetName();
    ImGui::SetDragDropPayload(DragDrop::GAsset, &AssetFileCached, sizeof(AAssetFile*));
    ImGui::Text("%s", AssetName.c_str());
    ImGui::EndDragDropSource();
}

void AssetManagerWindow::HilightItem(AAssetFile* AssetFileCached)
{
    const float YMargin = 5;
    const string& AssetName = AssetFileCached->GetAssetName();
    ImVec2 textSize = ImGui::CalcTextSize(AssetName.c_str(), nullptr, true, UISize::FileSize.x);
    ImVec2 childSize = ImVec2(UISize::FileSize.x, UISize::FileSize.y + textSize.y + YMargin);

    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ChildBg, UIColor::GHilighted);

    ImGui::BeginChild("Hilighted Asset Control", childSize, NULL, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar);
}

void AssetManagerWindow::UnhilightItem(AAssetFile* AssetFileCached)
{
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void AssetManagerWindow::RenderAssetFile(const path& AssetPathIn, const float& VisibleWidthIn)
{
    ImGuiStyle& Style = ImGui::GetStyle();
    const path& EntryFileName = AssetPathIn.filename();

    const string CurrentAssetName = EntryFileName.stem().string();
    const string CurrentAssetExtension = EntryFileName.extension().string();

    if (CurrentAssetExtension == AssetExtension)
    {
        AAssetFile* AssetFile = AssetManagerCached->GetManagingAsset(CurrentAssetName);

// юс╫ц
        AssetControl AssetControl(AssetFile);
        AssetControl.BeginDragDropEvent += OnAssetControlBeginDragDrop;
        AssetControl.HilightedEvent += OnAssetControlPushHilightStyle;
        AssetControl.UnhilightedEvent += OnAssetControlPopHilightStyle;

        AssetControl.RenderControl();

        float LastFinishedWidthPos = ImGui::GetItemRectMax().x;
        float NextStartWidthPos = LastFinishedWidthPos + Style.ItemSpacing.x + ImGui::GetItemRectSize().x;
        if (NextStartWidthPos < VisibleWidthIn) ImGui::SameLine();
    }
}

