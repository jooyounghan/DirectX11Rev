#include "GameWorld.h"

#include "GlobalVariable.h"
#include "AssetManager.h"

#include "PSOManager.h"
#include "PSOObject.h"
#include "MapAsset.h"
#include "DDSTextureAsset.h"
#include "BaseTextureAsset.h"

#include "ACamera.h"
#include "EnvironmentActor.h"
#include "EditorPawn.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
);

GameWorld::GameWorld(HWND WindowHandle)
{
	OnMapSelected = bind(&GameWorld::SetCurrentMap, this, placeholders::_1);
	OnIDSelected = bind(&GameWorld::SelectPlaceableByID, this, placeholders::_1);
	OnAssetDropped = bind(&GameWorld::AddAssetWithDropped, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
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


	TaskAnalyzerWindowInstance = AddDialog<TaskAnalyzerWindow>();
	MapOutlinerWindowInstance = AddDialog<MapOutlinerWindow>();


	AssetManagerWindowInstance = AddDialog<AssetManagerWindow>();
	
	MapOutlinerWindowInstance->IDSelectEvent += OnIDSelected;
	MapOutlinerWindowInstance->AssetDropEvent += OnAssetDropped;
	MapOutlinerWindowInstance->MapSelectedEvent += OnMapSelected;
}

void GameWorld::Update(const float& DeltaTimeIn)
{
	if (CurrentMap)
	{
		ACamera* CurrentCamera = CurrentMap->GetCurrentCamera();
		EditorPawn* EditorActorInstance = CurrentMap->GetEditorActorInstance();
		EnvironmentActor* EnvironmentActorInstance = CurrentMap->GetEnvironmentActorInstance();
		
		EditorActorInstance->Update(DeltaTimeIn);
		EnvironmentActorInstance->RelativePosition = EditorActorInstance->RelativePosition;
		EnvironmentActorInstance->Update(DeltaTimeIn);
		const list<unique_ptr<APlaceableObject>>& RootPlaceables = CurrentMap->GetRootPlaceables();

		BoundingFrustumObject* CameraFrustum = CurrentCamera->GetCamearaFrustum();
		for (auto& RootPlaceable : RootPlaceables)
		{
			RootPlaceable->Update(DeltaTimeIn);

			// TODO : 추후 Collision Manager로 변경
			const std::list<IIntersectable*>& Intersectables = RootPlaceable->GetIntersectables();
			for (auto& intersectable : Intersectables)
			{
				CollisionVisitor CollisionVisitorInstance(intersectable);
				if (CollisionVisitorInstance.Visit(CameraFrustum))
				{
					RootPlaceable->SetIsRenderable(true);
					break;
				}
				else
				{
					RootPlaceable->SetIsRenderable(false);
				}
			}
		}
	}
}

void GameWorld::Render()
{
	
	if (CurrentMap)
	{

		RenderingAlgorithm SelectedRenderingAlgorithm = TaskAnalyzerWindowInstance->GetSelectedRenderingAlgorithm();

		ACamera* CurrentCamera = CurrentMap->GetCurrentCamera();
		CurrentCamera->CleanupLens();
		
		EnvironmentActor* EnvironmentActorInstance = CurrentMap->GetEnvironmentActorInstance();
		EnvironmentActorInstance->Render(CurrentMap.get());

		const list<unique_ptr<APlaceableObject>>& RootPlaceables = CurrentMap->GetRootPlaceables();

		if (SelectedRenderingAlgorithm == RenderingAlgorithm::ForwardRendering)
		{
			for (auto& RootPlaceable : RootPlaceables)
			{
				if (RootPlaceable->GetIsRenderable())
				{
					RootPlaceable->Render(CurrentMap.get());
				}
			}
		}
		else if (SelectedRenderingAlgorithm == RenderingAlgorithm::DefferedRendering)
		{
			const list<unique_ptr<APlaceableObject>>& RootPlaceables = CurrentMap->GetRootPlaceables();

			for (auto& RootPlaceable : RootPlaceables)
			{
				if (RootPlaceable->GetIsRenderable())
				{
					//RootPlaceable->RenderDeffered(CurrentMap.get());
				}
			}

		}
		
		ResolveGBuffer();








		//MapOutlinerWindowInstance->GetSelectedPlaceable()->Render();
// 
		//CurrentCamera->PostProcess();
	}
	RenderUI();
}

void GameWorld::SetCurrentMap(const std::shared_ptr<MapAsset>& NewMap)
{
	CurrentMap = NewMap;
	MapOutlinerWindowInstance->SetCurrentCamera(NewMap->GetCurrentCamera());
}

void GameWorld::SelectPlaceableByID(const UINT& SelectedID)
{
	if (CurrentMap)
	{
		APlaceableObject* SelectedPlaceableObject = CurrentMap->GetSelectedPlaceableByID(SelectedID);
		MapOutlinerWindowInstance->SetSelectedPlaceable(SelectedPlaceableObject);
	}
}

void GameWorld::AddAssetWithDropped(
	AAssetFile* AssetFileIn, 
	const float& PosXIn, 
	const float& PosYIn, 
	const float& PosZIn
)
{
	if (CurrentMap)
	{
		CurrentMap->AddAsset(AssetFileIn, PosXIn, PosYIn, PosZIn);
	}
}



void GameWorld::AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
#endif
	ManageMessage(hWnd, msg, wParam, lParam);
}

void GameWorld::ResolveGBuffer()
{
	if (CurrentMap != nullptr)
	{
		ACamera* CurrentCamera = CurrentMap->GetCurrentCamera();
		EnvironmentActor* EnvironmentActorInstance =  CurrentMap->GetEnvironmentActorInstance();

		const D3D11_VIEWPORT* Viewport = &CurrentCamera->GetViewport();
		const std::vector<ID3D11Buffer*> VertexBuffers{
			CurrentCamera->GetResolveSquarePositionBuffer().GetBuffer(),
			CurrentCamera->GetResolveSquareUVBuffer	().GetBuffer()
		};
		const std::vector<UINT> Strides{ sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
		const std::vector<UINT> Offsets{ 0, 0 };

		ID3D11Buffer* IndexBuffer = CurrentCamera->GetResolveSquareIndexBuffer().GetBuffer();
		const UINT IndexTotalCount = CurrentCamera->GetResolveSquareIndexBuffer().GetIndicesCount();
		const DXGI_FORMAT IndexFormat = DXGI_FORMAT_R8_UINT;

		ID3D11DeviceContext* DeviceContextCached = App::GGraphicPipeline->GetDeviceContext();
		PSOObject* GBufferResolvePSOCached = App::GPSOManager->GetPSOObject(EPSOType::GBuffer_Resolve);

		DeviceContextCached->IASetIndexBuffer(IndexBuffer, IndexFormat, 0);
		DeviceContextCached->IASetVertexBuffers(0, static_cast<UINT>(VertexBuffers.size()),
			VertexBuffers.data(),
			Strides.data(),
			Offsets.data()
		);

		vector<ID3D11RenderTargetView*> RTVs = { CurrentCamera->GetSDRSceneRTV() };

		GBufferResolvePSOCached->SetPipelineStateObject(RTVs.size(), RTVs.data(), Viewport, CurrentCamera->GetGBufferDSV());

		vector<ID3D11Buffer*> PSConstants{ CurrentCamera->GetViewProjBuffer()->GetBuffer() };
		vector<ID3D11ShaderResourceView*> PSSRVs{
			EnvironmentActorInstance->GetEnvironmentSpecularDDSTextureAsset()->GetSRV(),
			EnvironmentActorInstance->GetEnvironmentDiffuseDDSTextureAsset()->GetSRV(),
			EnvironmentActorInstance->GetEnvironmentBRDFDDSTextureAsset()->GetSRV(),

			CurrentCamera->GetGBufferSRV(EGBuffer::Position_GBuffer),
			CurrentCamera->GetGBufferSRV(EGBuffer::BaseColor_GBuffer),
			CurrentCamera->GetGBufferSRV(EGBuffer::Normal_GBuffer),
			CurrentCamera->GetGBufferSRV(EGBuffer::AO_Metallic_Roughness_GBuffer),
			CurrentCamera->GetGBufferSRV(EGBuffer::Emissive_GBuffer),
		};

		GBufferResolvePSOCached->SetPSConstantBuffers(0, PSConstants.size(), PSConstants.data());
		GBufferResolvePSOCached->SetPSShaderResourceViews(0, PSSRVs.size(), PSSRVs.data());

		GBufferResolvePSOCached->CheckPipelineValidation();
		DeviceContextCached->DrawIndexed(IndexTotalCount, 0, 0);
		Performance::GTotalIndexCount += IndexTotalCount;

		GBufferResolvePSOCached->ResetPSConstantBuffers(0, PSConstants.size());
		GBufferResolvePSOCached->ResetPSShaderResourceViews(0, PSSRVs.size());
	}
}

void GameWorld::RenderUI()
{
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


template<typename T, typename ...Args>
inline T* GameWorld::AddDialog(Args ...args)
{
	static_assert(std::is_base_of<AWindow, T>::value, DerivedCondition(AWindow));

	Dialogs.emplace_back(make_unique<T>(args...));
	return (T*)Dialogs.back().get();
}
