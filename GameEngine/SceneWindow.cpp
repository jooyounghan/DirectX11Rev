#include "SceneWindow.h"

using namespace ImGui;
using namespace DirectX;

SceneWindow::SceneWindow(const std::string& windowID, PSOManager* psoManager, const uint32_t& width, const uint32_t& height)
	: AWindow(windowID), m_sceneRenderer(psoManager)
{
}

void SceneWindow::RenderWindowImpl()
{
    BeginGroup();
    PushID("RenderScene");

    ImVec2 regionAvail = GetContentRegionAvail();

    BeginChild("SceneViewport", ImVec2(regionAvail.x * 0.8f, regionAvail.y));
    RenderScene();
    EndChild();
    SameLine();
    BeginChild("ComponentHandler", ImVec2(regionAvail.x * 0.2f, regionAvail.y));
    RenderComponentTree();
    RenderComponentHandler();
    EndChild();

    PopID();
    EndGroup();
}

void SceneWindow::InitializeWindow(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
}

void SceneWindow::RenderScene()
{
    Image(nullptr, GetContentRegionAvail());
}

void SceneWindow::RenderComponentTree()
{
    ImVec2 regionAvail = GetContentRegionAvail();
    Button("RenderComponentTree", ImVec2(regionAvail.x, regionAvail.y * 0.6f));
}

void SceneWindow::RenderComponentHandler()
{
    Button("RenderComponentHandler", GetContentRegionAvail());
}
