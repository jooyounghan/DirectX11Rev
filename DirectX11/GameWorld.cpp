#include "GameWorld.h"

#include "GlobalVariable.h"
#include "AssetManager.h"
#include "MapAsset.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

using namespace std;
using namespace ImGui;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
);

GameWorld::GameWorld(HWND WindowHandle)
{
	InitEditor(WindowHandle);
}


GameWorld::~GameWorld()
{
}

void GameWorld::InitEditor(HWND WindowHandle)
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplWin32_Init(WindowHandle);
	ImGui_ImplDX11_Init(
		App::GGraphicPipeline->GetDevice(),
		App::GGraphicPipeline->GetDeviceContext()
	);

	AddDialog<TaskAnalyzerWindow>();
	ViewportWindowInstance = AddDialog<ViewportWindow>();
	
	MapOutlinerWindowInstance = AddDialog<MapOutlinerWindow>();
	OnMapSelected = bind(&GameWorld::SetCurrentMap, this, placeholders::_1);
	MapOutlinerWindowInstance->MapSelectedEvent += OnMapSelected;
	
	Dialogs.push_back(make_unique<AssetManagerWindow>());
}

void GameWorld::Update(const float& DeltaTimeIn)
{
	if (CurrentMap)
	{
		CurrentMap->Update(DeltaTimeIn);
	}
}

void GameWorld::SetCurrentMap(const std::shared_ptr<MapAsset>& NewMap)
{
	if (CurrentMap != nullptr)
	{
		ViewportWindowInstance->IDSelectEvent -= OnIDSelected;
		ViewportWindowInstance->AssetDropEvent -= OnAssetDropped;
	}

	CurrentMap = NewMap;
	
	if (NewMap != nullptr)
	{
		ViewportWindowInstance->SetCurrentCamera(NewMap->GetCurrentCamera());

		OnIDSelected = bind(&MapAsset::SetSelectedPlaceableByID, NewMap.get(), placeholders::_1);
		OnAssetDropped = bind(&MapAsset::AddAsset, NewMap.get(), placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

		ViewportWindowInstance->IDSelectEvent += OnIDSelected;
		ViewportWindowInstance->AssetDropEvent += OnAssetDropped;
	}

}

void GameWorld::Render()
{
	
	if (CurrentMap)
	{
		CurrentMap->Render();
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

	ID3D11RenderTargetView* BackBufferRTV = App::GGraphicPipeline->GetBackBufferRTV();
	ID3D11DeviceContext* DeviceContext = App::GGraphicPipeline->GetDeviceContext();

	DeviceContext->ClearRenderTargetView(BackBufferRTV, ClearColor);
	DeviceContext->OMSetRenderTargets(1, &BackBufferRTV, nullptr);

	ImGui::EndFrame();
	ImGui::UpdatePlatformWindows();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


void GameWorld::AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
#endif
	ManageMessage(hWnd, msg, wParam, lParam);
}

void GameWorld::ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DROPFILES:
	{
		HDROP hDrop = (HDROP)wParam;
		OnDropFiles(hDrop);
		break;
	}
	case NULL:
		break;
	default:
		break;
	}
}

void GameWorld::OnDropFiles(HDROP hDropIn)
{
	UINT fileCount = DragQueryFileA(hDropIn, 0xFFFFFFFF, NULL, 0);
	for (UINT i = 0; i < fileCount; ++i)
	{
		char filePath[MAX_PATH];
		if (DragQueryFileA(hDropIn, i, filePath, MAX_PATH))
		{
			string filePathStr = string(filePath);
			App::GAssetManager->LoadAssetFromFile(filePathStr);
		}
	}
	DragFinish(hDropIn);
}