#include "AssetManagerWindow.h"
#include "AssetManager.h"
#include "AAssetFile.h"

#include "AssetControlWindow.h"
#include "MaterialAssetNodeCanvas.h"

#include "UIVariable.h"

using namespace ImGui;
using namespace std;
using namespace filesystem;

AssetManagerWindow::AssetManagerWindow(AssetManager* AssetManagerIn)
	: AssetManagerCached(AssetManagerIn)
{
    RootDirectory.Name = "Assets";
    RootDirectory.Directory = ".\\Assets\\";

#pragma region Binding
    OnAssetChanged = bind(&AssetManagerWindow::RefreshAssetDirectoriesFromRoot, this);
    AssetManagerCached->AssetChangedEvent += OnAssetChanged;

    OnAssetControlBeginDragDrop = bind(&AssetManagerWindow::SetAssetControlDragDrop, this, placeholders::_1);

    OnAssetControlPushHilightStyle = bind(&AssetManagerWindow::HilightItem, this, placeholders::_1);
    OnAssetControlPopHilightStyle = bind(&AssetManagerWindow::UnhilightItem, this, placeholders::_1);

    OnAssetLeftMouseClicked = bind(&AssetManagerWindow::FocusItem, this, placeholders::_1);
    OnAssetLeftMouseDBClicked = bind(&AssetManagerWindow::OpenItemSetting, this, placeholders::_1);

    OnAssetControlWindowClosed = bind(&AssetManagerWindow::AddCloseAssetControlWindowList, this, placeholders::_1);
#pragma endregion 

    RefreshAssetDirectoriesFromRoot();
}

AssetManagerWindow::~AssetManagerWindow()
{
}

void AssetManagerWindow::RenderWindow()
{
	Begin("Asset Manager");

    RenderAssetDirectories();
    SameLine();
    RenderAssetControls();

    End();

    RenderAssetControlWindows();
}

void AssetManagerWindow::RenderAssetDirectories()
{
    ImVec2 RegionAvail = GetContentRegionAvail();
    BeginChild("Directories", ImVec2(RegionAvail.x * 0.2f, RegionAvail.y), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);
    RenderAssetDirectoriesHelper(RootDirectory);
    EndChild();
}

void AssetManagerWindow::RenderAssetDirectoriesHelper(DirectorySet& DirectorySetIn)
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
            RenderAssetDirectoriesHelper(ChildDirectory);
        }
        Unindent();
        TreePop();
    }
}

void AssetManagerWindow::RenderAssetControls()
{
    ImVec2 RegionAvail = GetContentRegionAvail();
    float MaxWidthForAssetControls = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    ImGuiStyle& Style = ImGui::GetStyle();

    BeginChild("Files", RegionAvail, ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);

    if (SelectedDirectory != nullptr)
    {
        for (AssetControl& AssetControlInstance : SelectedDirectory->AssetControls)
        {
            AssetControlInstance.RenderControl();


            float LastFinishedWidthPos = ImGui::GetItemRectMax().x;
            float NextStartWidthPos = LastFinishedWidthPos + Style.ItemSpacing.x + ImGui::GetItemRectSize().x;
            if (NextStartWidthPos < MaxWidthForAssetControls) ImGui::SameLine();
        }
    }

    EndChild();
}

void AssetManagerWindow::RenderAssetControlWindows()
{
    CloseAssetControlWindow();

    for (auto& AssetControlWindowCached : AssetControlWindows)
    {
        AssetControlWindowCached->RenderWindow();
    }
}

void AssetManagerWindow::RefreshAssetDirectoriesFromRoot()
{
    SelectedDirectory = nullptr;
    
    FocusItem(nullptr);

    RootDirectory.ChildrenDirectories.clear();
    RootDirectory.AssetControls.clear();

    BuildAssetDirectories(RootDirectory);
}

void AssetManagerWindow::BuildAssetDirectories(DirectorySet& DirectorySetIn)
{
    path Directory = DirectorySetIn.Directory;
    if (!exists(Directory) && create_directories(Directory)) {/* Do Nothing But Make Directory */ };

    for (const auto& entry : directory_iterator(Directory))
    {
        if (entry.is_directory())
        {
            list<DirectorySet>& ChildDirectories = DirectorySetIn.ChildrenDirectories;
            ChildDirectories.emplace_back(DirectorySet());

            DirectorySet& CurrentDirectory = ChildDirectories.back();

            const path& EntryPath = entry.path();
            CurrentDirectory.Name = EntryPath.filename().string();
            CurrentDirectory.Directory = EntryPath.string();

            BuildAssetDirectories(CurrentDirectory);
        }

        if (entry.is_regular_file())
        {
            path FilePath = path(entry);
            const string CurrentAssetName = FilePath.stem().string();
            const string CurrentAssetExtension = FilePath.extension().string();

            if (CurrentAssetExtension == AssetExtension)
            {
                AAssetFile* AssetFile = AssetManagerCached->GetManagingAsset(CurrentAssetName);
                DirectorySetIn.AssetControls.emplace_back(AssetFile);
                AssetControl& CurrentEmplacedControl = DirectorySetIn.AssetControls.back();

                CurrentEmplacedControl.BeginDragDropEvent += OnAssetControlBeginDragDrop;

                CurrentEmplacedControl.HilightedEvent += OnAssetControlPushHilightStyle;
                CurrentEmplacedControl.UnhilightedEvent+= OnAssetControlPopHilightStyle;

                CurrentEmplacedControl.LeftMouseClickedEvent += OnAssetLeftMouseClicked;
                CurrentEmplacedControl.LeftMouseDBClickedEvent+= OnAssetLeftMouseDBClicked;
            }
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

void AssetManagerWindow::HilightItem(AssetControl* AssetControlCached)
{
    const float YMargin = 5;
    AAssetFile* AssetFileCached = AssetControlCached->GetAssetFileCached();

    const string& AssetName = AssetFileCached->GetAssetName();
    ImVec2 textSize = ImGui::CalcTextSize(AssetName.c_str(), nullptr, true, UISize::FileSize.x);
    ImVec2 childSize = ImVec2(UISize::FileSize.x, UISize::FileSize.y + textSize.y + YMargin);

    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ChildBg, UIColor::GHilighted);

    ImGui::BeginChild("Hilighted Asset Control", childSize, NULL, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar);
}

void AssetManagerWindow::UnhilightItem(AssetControl* AssetControlCached)
{
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void AssetManagerWindow::FocusItem(AssetControl* AssetControlCached)
{
    if (SelectedAssetControl != nullptr)
    {
        SelectedAssetControl->SetFocus(false);
    }

    if (AssetControlCached != nullptr)
    {
        AssetControlCached->SetFocus(true);
    }
    SelectedAssetControl = AssetControlCached;
}

void AssetManagerWindow::OpenItemSetting(AssetControl* AssetControlCached)
{
    bool AssetControlWindowAdded = false;
    if (AssetControlCached != nullptr)
    {
        AAssetFile* AssetFile = AssetControlCached->GetAssetFileCached();
        if (AssetFile != nullptr)
        {
            EAssetType AssetType = AssetFile->GetAssetType();
            switch (AssetType)
            {
            case EAssetType::Material:
                AssetControlWindows.emplace_back(make_unique<AssetControlWindow<MaterialAssetNodeCanvas>>(AssetManagerCached, AssetFile));
                AssetControlWindowAdded = true;
                break;
            default:
                break;
            }
        }
    }

    if (AssetControlWindowAdded)
        AssetControlWindows.back()->CloseEvent += OnAssetControlWindowClosed;
}

void AssetManagerWindow::AddCloseAssetControlWindowList(AWindow* AssetControlWindowIn)
{ 
    CloseAssetControlWindowsList.push_back(AssetControlWindowIn); 
}

void AssetManagerWindow::CloseAssetControlWindow()
{
    for (AWindow* CloseAssetControlWindow : CloseAssetControlWindowsList)
    {
        AssetControlWindows.remove_if(
            [CloseAssetControlWindow](const unique_ptr<AWindow>& window)
            {
                return window.get() == CloseAssetControlWindow;
            }
        );
    }
    CloseAssetControlWindowsList.clear();
}
