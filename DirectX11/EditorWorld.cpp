#include "EditorWorld.h"

#include "GlobalVariable.h"

#include "DefineUtility.h"

#include "TaskAnalyzerDialog.h"
#include "ViewportDialog.h" 
#include "MapOutlinerDialog.h"

#include "GameWorld.h"
#include "GraphicsPipeline.h"

#include "Camera.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

using namespace std;


EditorWorld::EditorWorld(GameWorld* GameWorldIn, HWND WindowHandle)
    : IWorld(GameWorldIn->GraphicsPipelineCached), GameWorldCached(GameWorldIn)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());

    
    ImGui_ImplWin32_Init(WindowHandle);
    ImGui_ImplDX11_Init(
        GraphicsPipelineCached->GetDevice(),
        GraphicsPipelineCached->GetDeviceContext()
    );

    Dialogs.push_back(make_unique<TaskAnalyzerDialog>());
    Dialogs.push_back(make_unique<ViewportDialog>(GameWorldCached));
    Dialogs.push_back(make_unique<MapOutlinerDialog>(GameWorldCached));

    EditorCamera = make_unique<Camera>(
        GraphicsPipelineCached, App::GWidth, App::GHeight
    );
}

EditorWorld::~EditorWorld()
{
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

void EditorWorld::RenderWorld()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    for (auto& Dialog : Dialogs)
    {
        Dialog->DoModal();
    }

    ImGui::Render();

    const FLOAT ClearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };

    ID3D11RenderTargetView* BackBufferRTV = GraphicsPipelineCached->GetBackBufferRTV();
    ID3D11DeviceContext* DeviceContext = GraphicsPipelineCached->GetDeviceContext();

    DeviceContext->ClearRenderTargetView(BackBufferRTV, ClearColor);
    DeviceContext->OMSetRenderTargets(1, &BackBufferRTV, nullptr);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

LRESULT EditorWorld::AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        return 0;
    case WM_EXITSIZEMOVE:
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
