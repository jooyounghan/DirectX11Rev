#include "AssetManagerWindow.h"

#include "GlobalVariable.h"
#include "AssetManager.h"
#include "AAssetFile.h"

#include "AssetControlWindow.h"
#include "MaterialAssetNodeCanvas.h"

#include "CreateMaterialAssetModal.h"
#include "CreateRetargetedAnimationAssetModal.h"

#include "MapAsset.h"
#include "BaseTextureAsset.h"
#include "DDSTextureAsset.h"
#include "EXRTextureAsset.h"
#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"
#include "BoneAsset.h"
#include "AnimationAsset.h"
#include "MaterialAsset.h"


#include "UIVariable.h"

using namespace std;
using namespace ImGui;
using namespace filesystem;

AssetManagerWindow::AssetManagerWindow()
	: AddAssetFilePopupInstance(format("AddAssetFilePopupInstance{}", (uint64_t)this))    
{
    RootDirectory.Name = "Assets";
    RootDirectory.Directory = ".\\Assets\\";

    CreateAssetModalInstances.emplace("CreateMaterialAssetModalInstance", make_unique<CreateMaterialAssetModal>("Create Material Asset"));
    CreateAssetModalInstances.emplace("CreateRTAnimationAssetModalInstance", make_unique<CreateRetargetedAnimationAssetModal>("Create Retargeted Animation Asset"));

#pragma region Binding
    OnAssetChanged = bind(&AssetManagerWindow::RefreshAssetDirectoriesFromRoot, this);
    App::GAssetManager->AssetChangedEvent += OnAssetChanged;

    OnAssetControlBeginDragDrop = bind(&AssetManagerWindow::SetAssetControlDragDrop, this, placeholders::_1);

    OnAssetControlPushHilightStyle = bind(&AssetManagerWindow::StartHilightItem, this, placeholders::_1);
    OnAssetControlPopHilightStyle = bind(&AssetManagerWindow::EndHilightItem, this, placeholders::_1);

    OnAssetLeftMouseClicked = bind(&AssetManagerWindow::FocusItem, this, placeholders::_1);
    OnAssetLeftMouseDBClicked = bind(&AssetManagerWindow::OpenItemSetting, this, placeholders::_1);

    OnAssetControlWindowClosed = bind(&AssetManagerWindow::AddCloseAssetControlWindowList, this, placeholders::_1);

    OnCreateMaterialAsset = bind(&AssetManagerWindow::StartCreateMaterialAsset, this);
    OnCreateRetargetedAnimationAsset = bind(&AssetManagerWindow::StartCreateRetargetedAnimationAsset, this);

    AddAssetFilePopupInstance.MaterialAssetCreated += OnCreateMaterialAsset;
    AddAssetFilePopupInstance.RetargetedAnimationCreated += OnCreateRetargetedAnimationAsset;
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

    if (RegionAvail.x > 0 && RegionAvail.y > 0)
    {
        ImVec2 CurrentCursorPos = GetCursorScreenPos();
        AddAssetFilePopupInstance.PopUp(CurrentCursorPos, ImVec2(CurrentCursorPos.x + RegionAvail.x, CurrentCursorPos.y + RegionAvail.y));

        for (auto& CreateAssetModalInstance : CreateAssetModalInstances)
        {
            CreateAssetModalInstance.second->DoModal();
        }

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

    // TODO : 하드코딩 수정
    const shared_ptr<BaseTextureAsset>& BaseTextureThumbnail = App::GAssetManager->GetManagingBasicTexture("BaseTextureThumbnail_BaseTexture");
    const shared_ptr<BaseTextureAsset>& BaseStaticThumbnail = App::GAssetManager->GetManagingBasicTexture("BaseStaticMeshThumbnail_BaseTexture");
    const shared_ptr<BaseTextureAsset>& BaseSkeletalThumbnail = App::GAssetManager->GetManagingBasicTexture("BaseSkeletalMeshThumbnail_BaseTexture");
    const shared_ptr<BaseTextureAsset>& BaseBoneThumbnail = App::GAssetManager->GetManagingBasicTexture("BaseBoneThumbnail_BaseTexture");
    const shared_ptr<BaseTextureAsset>& BaseAnimationThumbnail = App::GAssetManager->GetManagingBasicTexture("BaseAnimationThumbnail_BaseTexture");
    const shared_ptr<BaseTextureAsset>& BaseMapThumbnail = App::GAssetManager->GetManagingBasicTexture("BaseMapThumbnail_BaseTexture");
    const shared_ptr<BaseTextureAsset>& BaseMaterialThumbnail = App::GAssetManager->GetManagingBasicTexture("BaseMaterialThumbnail_BaseTexture");


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
                AAssetFile* AssetFile = App::GAssetManager->GetManagingAsset(CurrentAssetName);

                if (AssetFile != nullptr)
                {
                    const string& AssetType = AssetFile->GetAssetType();

                    ID3D11ShaderResourceView* ThumbnailSRV = nullptr;
                    if (AssetType == BaseTextureAsset::BaseTextureAssetKind || AssetType == EXRTextureAsset::EXRTextureAssetKind)
                    {
                        ThumbnailSRV = AssetFile->GetThumbnailSRV();
                    }
                    else if (AssetType == DDSTextureAsset::DDSTextureAssetKind)
                    {
                        ID3D11ShaderResourceView* SRV = AssetFile->GetThumbnailSRV();
                        D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
                        SRV->GetDesc(&SRVDesc);
                        if (SRVDesc.ViewDimension == D3D11_SRV_DIMENSION_TEXTURE2D)
                        {
                            ThumbnailSRV = SRV;
                        }
                        else
                        {
                            ThumbnailSRV = BaseTextureThumbnail != nullptr ? BaseTextureThumbnail->GetThumbnailSRV() : nullptr;
                        }
                    }
                    else if (AssetType == StaticMeshAsset::StaticMeshAssetKind)
                    {
                        ThumbnailSRV = BaseStaticThumbnail != nullptr ? BaseStaticThumbnail->GetThumbnailSRV() : nullptr;
                    }
                    else if (AssetType == SkeletalMeshAsset::SkeletalMeshAssetKind)
                    {
                        ThumbnailSRV = BaseSkeletalThumbnail != nullptr ? BaseSkeletalThumbnail->GetThumbnailSRV() : nullptr;
                    }
                    else if (AssetType == BoneAsset::BoneAssetKind)
                    {
                        ThumbnailSRV = BaseBoneThumbnail != nullptr ? BaseBoneThumbnail->GetThumbnailSRV() : nullptr;
                    }
                    else if (AssetType == AnimationAsset::AnimationKind)
                    {
                        ThumbnailSRV = BaseAnimationThumbnail != nullptr ? BaseAnimationThumbnail->GetThumbnailSRV() : nullptr;
                    }
                    else if (AssetType == MapAsset::MapAssetKind)
                    {
                        ThumbnailSRV = BaseMapThumbnail != nullptr ? BaseMapThumbnail->GetThumbnailSRV() : nullptr;
                    }
                    else if (AssetType == MaterialAsset::MaterialAssetKind)
                    {
                        ThumbnailSRV = BaseMaterialThumbnail != nullptr ? BaseMaterialThumbnail->GetThumbnailSRV() : nullptr;
                    }
                    else;

                    DirectorySetIn.AssetControls.emplace_back(AssetFile, ThumbnailSRV);


                    AssetControl& CurrentEmplacedControl = DirectorySetIn.AssetControls.back();

                    CurrentEmplacedControl.BeginDragDropEvent += OnAssetControlBeginDragDrop;

                    CurrentEmplacedControl.StartHilightedEvent += OnAssetControlPushHilightStyle;
                    CurrentEmplacedControl.EndHilightedEvent += OnAssetControlPopHilightStyle;

                    CurrentEmplacedControl.LeftMouseClickedEvent += OnAssetLeftMouseClicked;
                    CurrentEmplacedControl.LeftMouseDBClickedEvent += OnAssetLeftMouseDBClicked;
                }
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

void AssetManagerWindow::StartHilightItem(AssetControl* AssetControlCached)
{
    const float YMargin = 5;
    AAssetFile* AssetFileCached = AssetControlCached->GetAssetFileCached();

    const string& AssetName = AssetFileCached->GetAssetName();
    ImVec2 textSize = ImGui::CalcTextSize(AssetName.c_str(), nullptr, true, UISize::FileSize.x);
    ImVec2 childSize = ImVec2(UISize::FileSize.x, UISize::FileSize.y + textSize.y + YMargin);

    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ChildBg, UIColor::GHilighted);

    ImGui::BeginChild("Hilighted Asset Control", childSize, NULL, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar);
}

void AssetManagerWindow::EndHilightItem(AssetControl* AssetControlCached)
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
            const string& AssetType = AssetFile->GetAssetType();
            if (AssetType == MaterialAsset::MaterialAssetKind)
            {
                AssetControlWindows.emplace_back(make_unique<AssetControlWindow<MaterialAssetNodeCanvas>>(AssetFile));
                AssetControlWindowAdded = true;
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

void AssetManagerWindow::StartCreateMaterialAsset()
{
    if (CreateAssetModalInstances.find("CreateMaterialAssetModalInstance") != CreateAssetModalInstances.end())
    {
        CreateAssetModalInstances["CreateMaterialAssetModalInstance"]->SetModalFlag(true);
    }
}

void AssetManagerWindow::StartCreateRetargetedAnimationAsset()
{
    if (CreateAssetModalInstances.find("CreateRTAnimationAssetModalInstance") != CreateAssetModalInstances.end())
    {
        CreateAssetModalInstances["CreateRTAnimationAssetModalInstance"]->SetModalFlag(true);
    }
}
