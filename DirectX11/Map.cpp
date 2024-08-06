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
#include "BoundingFrustum.h"
#include "CollisionVisitor.h"

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

		Ray ClickedRay = Ray::CreateRay(ScreenXIn, ScreenYIn, ScreenWidthIn, ScreenHeightIn, MapCamera->GetProjectionMatrix(), MapCamera->GetViewMatrix());

		const XMVECTOR PlacePositon = ClickedRay.Origin + ClickedRay.Direction * (500.f);

		AddedObject->Position.x = PlacePositon.m128_f32[0];
		AddedObject->Position.y = PlacePositon.m128_f32[1];
		AddedObject->Position.z = PlacePositon.m128_f32[2];
		PSOToObjects[Pso].emplace_back(AddedObject);

		// TEST
		PSOObject* DebugPSO = PSOManagerCached->GetPSOObject(EPSOType::R8G8B8A8_Debug_Wireframe);

		OrientedBoundingBox* OBB = AddedObject->AddChildObjectHelper<OrientedBoundingBox>(GraphicsPipelineCached, AssetManagerCached,
			100.f, 100.f, 100.f);
		PSOToObjects[DebugPSO].emplace_back(OBB);
		OBB->Position.x += 400.f;

		BoundingSphere* BS = OBB->AddChildObjectHelper<BoundingSphere>(GraphicsPipelineCached, AssetManagerCached, 100.f);
		PSOToObjects[DebugPSO].emplace_back(BS);
		BS->Position.x += 400.f;

		Tests.push_back(OBB);
		Tests.push_back(BS);

		IIntersectable* Test1 = (IIntersectable*)OBB;
		IIntersectable* Test2 = (IIntersectable*)BS;

		CollisionVisitor Visitor(Test1);
		Test2->AcceptCollision(&Visitor);

	}
}

void Map::UpdateMap(const float& DeltaTimeIn)
{
	// Test
	BoundingFrustum Frustum(GraphicsPipelineCached, AssetManagerCached, MapCamera.get());
	Frustum.UpdateObject(DeltaTimeIn);
	
	for (auto& i : Tests)
	{
		CollisionVisitor Visitor(i);
		if (Frustum.AcceptCollision(&Visitor))
		{
			printf("OK\n");
		}
	}
	printf("Updated\n");



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

void Map::Test(float ScreenXIn, float ScreenYIn, float ScreenWidthIn, float ScreenHeightIn)
{
	Ray ClickedRay = Ray::CreateRay(ScreenXIn, ScreenYIn, ScreenWidthIn, ScreenHeightIn, MapCamera->GetProjectionMatrix(), MapCamera->GetViewMatrix());

	float Distance = 0;
	for (auto test : Tests)
	{
		if (test->Intersect(&ClickedRay, Distance))
		{
			bool test = true;
		}
	}
}
