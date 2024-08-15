#include "EditorWorld.h"

#include "GlobalVariable.h"
#include "DefineUtility.h"

#include "EditorCamera.h"

#include "TaskAnalyzerWindow.h"
#include "ViewportWindow.h" 
#include "MapOutlinerWindow.h"
#include "AssetManagerWindow.h"

#include "AssetManager.h"
#include "PSOManager.h"

#include "GameWorld.h"
#include "Map.h"
#include "GraphicsPipeline.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

using namespace std;


EditorWorld::EditorWorld(GameWorld* GameWorldIn, HWND WindowHandle)
    : IWorld(GameWorldIn->GraphicsPipelineCached), GameWorldCached(GameWorldIn)
{
    EditorCameraInstance = make_unique<EditorCamera>(
        GameWorldIn->GraphicsPipelineCached, App::GWidth, App::GHeight
    );

    // Test =============================================================
    EditorCameraInstance->Position = SPosition4D{ 0.f, 0.f, -300.f, 1.f };
    // =============================================================

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplWin32_Init(WindowHandle);
    ImGui_ImplDX11_Init(
        GraphicsPipelineCached->GetDevice(),
        GraphicsPipelineCached->GetDeviceContext()
    );

    Dialogs.push_back(make_unique<TaskAnalyzerWindow>());
    Dialogs.push_back(make_unique<ViewportWindow>(this));
    Dialogs.push_back(make_unique<MapOutlinerWindow>(this));
    Dialogs.push_back(make_unique<AssetManagerWindow>(GameWorldCached->GetAssetManagerInstance()));
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

void EditorWorld::SetSelecteObjectByID(const UINT& Id)
{
    Map* CurrentMap = GameWorldCached->GetCurrentMap();
    if (CurrentMap != nullptr)
    {
        const unordered_map<UINT, APlaceable*>& IdToPlaceables = CurrentMap->GetIdToPlaceables();
        if (IdToPlaceables.find(Id) != IdToPlaceables.end())
        {
            SelectedObject = (AObject*)IdToPlaceables.at(Id);
        }
    }
}

void EditorWorld::UpdateWorld(const float& DeltaTimeIn)
{
    EditorCameraInstance->UpdateObject(DeltaTimeIn);
}

void EditorWorld::RenderWorld()
{
    EditorCameraInstance->CleanupLens();
    Map* CurrentMap = GameWorldCached->GetCurrentMap();
    PSOManager* PSOManagerInstance = GameWorldCached->GetPSOManagerInstance();
    if (CurrentMap != nullptr)
    {
        ARenderer* PickingIDRenderers[] =
        {
            PSOManagerInstance->GetRenderers(EPSOType::R8G8B8A8_Picking_ID_Solid),
            PSOManagerInstance->GetRenderers(EPSOType::R8G8B8A8_Picking_ID_Wireframe)
        };

        const list<unique_ptr<APlaceable>>& RootPlaceables = CurrentMap->GetRootPlaceables();

        ID3D11RenderTargetView* RTVs[] = { EditorCameraInstance->GetIdSelectRTV() };
        D3D11_VIEWPORT Viewports[] = { EditorCameraInstance->GetViewport() };
        ID3D11DepthStencilView* DSV = EditorCameraInstance->GetIdSelectDSV();

        for (ARenderer* PickingIDRenderer : PickingIDRenderers)
        {
            PickingIDRenderer->PresetRendering(1, RTVs, Viewports, DSV, EditorCameraInstance.get(), CurrentMap);
            for (auto& Placeable : RootPlaceables)
            {
                Placeable->AcceptRenderer(PickingIDRenderer);
            }
            PickingIDRenderer->ResetRendering();
        }
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    for (auto& Dialog : Dialogs)
    {
        Dialog->RenderWindow();
    }

    ImGui::Render();

    const FLOAT ClearColor[] = { 0.5f, 0.2f, 0.4f, 1.0f };

    ID3D11RenderTargetView* BackBufferRTV = GraphicsPipelineCached->GetBackBufferRTV();
    ID3D11DeviceContext* DeviceContext = GraphicsPipelineCached->GetDeviceContext();

    DeviceContext->ClearRenderTargetView(BackBufferRTV, ClearColor);
    DeviceContext->OMSetRenderTargets(1, &BackBufferRTV, nullptr);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void EditorWorld::AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
    ManageMessage(hWnd, msg, wParam, lParam);
}

void EditorWorld::ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_EXITSIZEMOVE:
        break;
    case WM_DROPFILES:
    {
        HDROP hDrop = (HDROP)wParam;
        OnDropFiles(hDrop);
        break;
    }
    default:
        break;
    }
}

void EditorWorld::OnDropFiles(HDROP hDropIn)
{
    UINT fileCount = DragQueryFileA(hDropIn, 0xFFFFFFFF, NULL, 0);
    for (UINT i = 0; i < fileCount; ++i)
    {
        char filePath[MAX_PATH];
        if (DragQueryFileA(hDropIn, i, filePath, MAX_PATH))
        {
            string filePathStr = string(filePath);
            GameWorldCached->GetAssetManagerInstance()->LoadModelFile(filePathStr);
        }
    }
    DragFinish(hDropIn);
}
