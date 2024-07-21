#include "AssetManagerWindow.h"
#include "AssetManager.h"

AssetManagerWindow::AssetManagerWindow(AssetManager* AssetManagerIn)
	: AssetManagerCached(AssetManagerIn)
{
}

AssetManagerWindow::~AssetManagerWindow()
{
}

void AssetManagerWindow::RenderWindow()
{
	ImGui::Begin("Asset Manager");
	RenderAssetFileStructure();
    ImGui::SameLine();
    RenderCurrentDirectoryAsset();
	ImGui::End();
}

void AssetManagerWindow::RenderAssetFileStructure()
{
    ImVec2 RegionAvail = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("Director", ImVec2(RegionAvail.x * 0.2f, RegionAvail.y), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
        if (ImGui::TreeNode("Advanced, with Selectable nodes"))
        {
            static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed;
            ImGui::TreePop();
        }
    ImGui::EndChild();
}

void AssetManagerWindow::RenderCurrentDirectoryAsset()
{
    ImVec2 RegionAvail = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("Director1", RegionAvail, ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::TreeNode("Advanced, with Selectable nodes1"))
    {
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed;
        ImGui::TreePop();
    }
    ImGui::EndChild();
}
