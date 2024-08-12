#include "Map.h"

#include "GraphicsPipeline.h"

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
		MeshObject* AddedObject = PlaceableAddHelper<MeshObject>(GraphicsPipelineCached, MeshAssetIn);

		AddedObject->Position.x = PosXIn;
		AddedObject->Position.y = PosYIn;
		AddedObject->Position.z = PosZIn;

		// TEST ========================================================================================================
		OrientedBoundingBox* OBB = AddedObject->AddAttachedObjectHelper<OrientedBoundingBox>(GraphicsPipelineCached, AssetManagerCached,
			100.f, 100.f, 100.f);
		OBB->Position.x += 400.f;

		BoundingSphere* BB = OBB->AddAttachedObjectHelper<BoundingSphere>(GraphicsPipelineCached, AssetManagerCached,
			100.f);
		OBB->Position.x += 400.f;

		Tests.push_back(OBB);
		Tests.push_back(BB);
		// TEST ========================================================================================================
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
	vector<ARenderer*> Renderers = {
		PSOManagerCached->GetRenderers(EPSOType::R8G8B8A8_Skeletal_Solid),
		PSOManagerCached->GetRenderers(EPSOType::R8G8B8A8_BoundingComponent_Wireframe)
	};

	ID3D11RenderTargetView* RTVs[] = { CameraCached->GetSceneRTV() };
	D3D11_VIEWPORT Viewports[] = { CameraCached->GetViewport() };
	ID3D11DepthStencilView* DSV = CameraCached->GetSceneDSV();

	for (auto& Renderer : Renderers)
	{
		Renderer->PresetRendering(1, RTVs, Viewports, DSV, CameraCached, this);

		for (const auto& ro : Placeables)
		{
			ro->AcceptRenderer(Renderer);
		}

		Renderer->ResetRendering();
	}
}

void Map::Serialize(const std::string& OutputAdditionalPath)
{
}

void Map::Deserialize(FILE* FileIn, ID3D11Device* DeviceIn)
{
}
