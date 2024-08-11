#include "Map.h"
#include "GlobalVariable.h"

#include "PSOManager.h"
#include "PSOObject.h"
#include "AssetManager.h"

#include "MeshObject.h"
#include "AMeshAsset.h"

#include "Camera.h"

// TEST
#include "EditorCamera.h"
#include "BoundingSphere.h"
#include "OrientedBoundingBox.h"
#include "BoundingFrustum.h"
#include "CollisionVisitor.h"

using namespace std;
using namespace DirectX;

Map::Map(GraphicsPipeline* GraphicsPipelineInstance, PSOManager* PSOManagerInstance, AssetManager* AssetManagerInstance)
	: GraphicsPipelineCached(GraphicsPipelineInstance), PSOManagerCached(PSOManagerInstance), AssetManagerCached(AssetManagerInstance)
{

}

Map::~Map()
{
}

void Map::AddRenderObject(AMeshAsset* MeshAssetIn, float PosXIn, float PosYIn, float PosZIn)
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

		// TEST ========================================================================================================
		PSOObject* DebugPSO = PSOManagerCached->GetPSOObject(EPSOType::R8G8B8A8_Debug_Wireframe);

		OrientedBoundingBox* OBB = AddedObject->AddAttachedObjectHelper<OrientedBoundingBox>(GraphicsPipelineCached, AssetManagerCached,
			100.f, 100.f, 100.f);
		PSOToObjects[DebugPSO].emplace_back(OBB);
		OBB->Position.x += 400.f;

		BoundingSphere* BB = OBB->AddAttachedObjectHelper<BoundingSphere>(GraphicsPipelineCached, AssetManagerCached,
			100.f);

		PSOToObjects[DebugPSO].emplace_back(OBB);
		PSOToObjects[DebugPSO].emplace_back(BB);

		OBB->Position.x += 400.f;

		Tests.push_back(OBB);
		Tests.push_back(BB);
		// TEST ========================================================================================================
	}
}


void Map::UpdateMap(const float& DeltaTimeIn)
{
	for (auto& ro : Placeables)
	{
		ro->UpdateObject(DeltaTimeIn);
	}
}

void Map::RenderMap()
{
	for (auto& PSOToRendersPair : PSOToObjects)
	{
		PSOObject* Pso = PSOToRendersPair.first;
		ID3D11RenderTargetView* RTVs[] = { CameraCached->GetSceneRTV() };
		D3D11_VIEWPORT Viewports[] = { CameraCached->GetViewport() };
		ID3D11DepthStencilView* DSV = CameraCached->GetSceneDSV();
		Pso->SetPipelineObject(1, RTVs, Viewports, DSV);
	
		ID3D11Buffer* ViewProjBuffer[] = { CameraCached->ViewProjBuffer.GetBuffer() };
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
