#include "Map.h"
#include "GlobalVariable.h"

#include "PSOManager.h"
#include "PSOObject.h"
#include "AssetManager.h"

#include "MeshObject.h"
#include "IMeshAsset.h"

#include "Camera.h"

// TEST
#include "BoundingSphere.h"
#include "OrientedBoundingBox.h"

using namespace std;
using namespace DirectX;

Map::Map(GraphicsPipeline* GraphicsPipelineInstance, PSOManager* PSOManagerInstance, AssetManager* AssetManagerInstance)
	: GraphicsPipelineCached(GraphicsPipelineInstance), PSOManagerCached(PSOManagerInstance), AssetManagerCached(AssetManagerInstance)
{

	MapCamera = make_unique<Camera>(
		GraphicsPipelineCached, App::GWidth, App::GHeight
	);

	// TEST
	MapCamera->Position = SPosition4D{ 0.f, 0.f, -300.f, 1.f };
}

Map::~Map()
{
}

void Map::AddRenderObject(IMeshAsset* MeshAssetIn, float PosXIn, float PosYIn, float PosZIn)
{
	PSOObject* Pso = nullptr;

	switch (MeshAssetIn->GetAssetType())
	{
	case(EAssetType::StaticMesh):
	{
		Pso = PSOManagerCached->GetPSOObject(EPSOType::R8G8B8A8_Static_Solid);
		break;
	}
	case(EAssetType::SkeletalMesh):
	{
		Pso = PSOManagerCached->GetPSOObject(EPSOType::R8G8B8A8_Skeletal_Solid);		
		break;
	}
	default:
		break;
	}

	if (Pso != nullptr)
	{
		MeshObject* AddedObject = RenderableAddHelper<MeshObject>(GraphicsPipelineCached, MeshAssetIn);
		AddedObject->Position.x = PosXIn;
		AddedObject->Position.y = PosYIn;
		AddedObject->Position.z = PosZIn;

		PSOToObjects[Pso].emplace_back(AddedObject);
	}
}

void Map::AddRenderObject(IMeshAsset* MeshAssetIn, float ScreenXIn, float ScreenYIn, float ScreenWidthIn, float ScreenHeightIn)
{
	PSOObject* Pso = nullptr;

	switch (MeshAssetIn->GetAssetType())
	{
	case(EAssetType::StaticMesh):
	{
		Pso = PSOManagerCached->GetPSOObject(EPSOType::R8G8B8A8_Static_Solid);
		break;
	}
	case(EAssetType::SkeletalMesh):
	{
		Pso = PSOManagerCached->GetPSOObject(EPSOType::R8G8B8A8_Skeletal_Solid);
		break;
	}
	default:
		break;
	}

	if (Pso != nullptr)
	{
		MeshObject* AddedObject = RenderableAddHelper<MeshObject>(GraphicsPipelineCached, MeshAssetIn);

		XMVECTOR NearPoint = XMVectorSet(
			(2.0f * ScreenXIn / ScreenWidthIn) - 1.0f,
			1.0f - (2.0f * ScreenYIn / ScreenHeightIn),
			0.f, 1.f
		);

		XMVECTOR FarPoint = XMVectorSet(
			(2.0f * ScreenXIn / ScreenWidthIn) - 1.0f,
			1.0f - (2.0f * ScreenYIn / ScreenHeightIn),
			1.f, 1.f
		);

		const XMMATRIX ProjInvMatrix = XMMatrixInverse(nullptr, MapCamera->GetProjectionMatrix());
		NearPoint = XMVector3TransformCoord(NearPoint, ProjInvMatrix);
		FarPoint = XMVector3TransformCoord(FarPoint, ProjInvMatrix);

		const XMMATRIX ViewInvMatrix = XMMatrixInverse(nullptr, MapCamera->GetViewMatrix());
		NearPoint = XMVector3TransformCoord(NearPoint, ViewInvMatrix);
		FarPoint = XMVector3TransformCoord(FarPoint, ViewInvMatrix);

		const XMVECTOR Displacement = FarPoint - NearPoint;
		const XMVECTOR Direction = XMVector3Normalize(Displacement);
		const XMVECTOR PlacePositon = NearPoint + Direction * (Displacement.m128_f32[2] / 2.f);

		AddedObject->Position.x = PlacePositon.m128_f32[0];
		AddedObject->Position.y = PlacePositon.m128_f32[1];
		AddedObject->Position.z = PlacePositon.m128_f32[2];
		PSOToObjects[Pso].emplace_back(AddedObject);

		// TEST
		PSOObject* DebugPSO = PSOManagerCached->GetPSOObject(EPSOType::R8G8B8A8_Debug_Wireframe);

		OrientedBoundingBox* OBB = AddedObject->AddChildObjectHelper<OrientedBoundingBox>(GraphicsPipelineCached, AssetManagerCached,
			10.f, 10.f, 10.f);
		PSOToObjects[DebugPSO].emplace_back(OBB);
		OBB->Position.x += 200.f;

		BoundingSphere* BS = OBB->AddChildObjectHelper<BoundingSphere>(GraphicsPipelineCached, AssetManagerCached, 10.f);
		PSOToObjects[DebugPSO].emplace_back(BS);
		BS->Position.x += 400.f;
	}
}

void Map::UpdateMap(const float& DeltaTimeIn)
{
	MapCamera->UpdateObject(DeltaTimeIn);

	for (auto& ro : Placeables)
	{
		ro->UpdateObject(DeltaTimeIn);
	}
}

void Map::RenderMap()
{
	MapCamera->CleanupLens();

	for (auto& PSOToRendersPair : PSOToObjects)
	{
		PSOObject* Pso = PSOToRendersPair.first;
		ID3D11RenderTargetView* RTVs[] = { MapCamera->GetSceneRTV() };
		D3D11_VIEWPORT Viewports[] = { MapCamera->GetViewport() };
		ID3D11DepthStencilView* DSV = MapCamera->GetSceneDSV();
		Pso->SetPipelineObject(1, RTVs, Viewports, DSV);
	
		ID3D11Buffer* ViewProjBuffer[] = { MapCamera->ViewProjBuffer.GetBuffer() };
		Pso->SetVSConstantBuffers(0, 1, ViewProjBuffer);
		for (const auto& ro : PSOToRendersPair.second)
		{
			ro->Render(Pso);
		}
		Pso->ResetVSConstantBuffers(0, 1);
	}
}

void Map::Serialize(const std::string& OutputAdditionalPath)
{
}

void Map::Deserialize(FILE* FileIn, ID3D11Device* DeviceIn)
{
}
