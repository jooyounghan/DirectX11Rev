#include "AssetControlWindow.h"

#include "GlobalVariable.h"

#include "AssetControl.h"
#include "AAssetFile.h"

#include "NodeCanvas.h"

#include <format>

using namespace std;
using namespace ImGui;

AssetControlWindow::AssetControlWindow(AssetControl* AssetControlCachedIn)
	: AssetControlCached(AssetControlCachedIn), WindowSize(UISize::WindowSize)
{
    AAssetFile* AssetFileCached = AssetControlCached->GetAssetFileCached();
    if (AssetFileCached != nullptr)
    {
        WindowDescription = "Asset Control Window For " + AssetFileCached->GetAssetName();
    }
    else
    {
        WindowDescription = string(format("Asset Control Window For {}", (uint64_t)AssetControlCachedIn));
    }
}

void AssetControlWindow::RenderWindow()
{
    ImGui::SetNextWindowSizeConstraints(UISize::WindowSize, ImVec2(FLT_MAX, FLT_MAX));

    Begin(WindowDescription.c_str(), &bIsOpen, ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings);
    static NodeCanvas nodeCanvas;
    nodeCanvas.RenderControl();
    End();

    if (!bIsOpen)
    {
        return CloseEvent.Invoke(this);
    }
}
