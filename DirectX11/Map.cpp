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
	ARenderer* Renderer = nullptr;

	switch (MeshAssetIn->GetAssetType())
	{
	case(EAssetType::StaticMesh):
	{
		Renderer = PSOManagerCached->GetRenderers(EPSOType::R8G8B8A8_Static_Solid);
		break;
	}
	case(EAssetType::SkeletalMesh):
	{
		Renderer = PSOManagerCached->GetRenderers(EPSOType::R8G8B8A8_Skeletal_Solid);
		break;
	}
	default:
		break;
	}

	if (Renderer != nullptr)
	{
		MeshObject* AddedObject = RenderableAddHelper<MeshObject>(GraphicsPipelineCached, MeshAssetIn);

		AddedObject->Position.x = PosXIn;
		AddedObject->Position.y = PosYIn;
		AddedObject->Position.z = PosZIn;

		RendererToObjects[Renderer].emplace_back(AddedObject);

		// TEST ========================================================================================================
		ARenderer* BoundingComponentPSO = PSOManagerCached->GetRenderers(EPSOType::R8G8B8A8_BoundingComponent_Wireframe);

		OrientedBoundingBox* OBB = AddedObject->AddAttachedObjectHelper<OrientedBoundingBox>(GraphicsPipelineCached, AssetManagerCached,
			100.f, 100.f, 100.f);
		RendererToObjects[BoundingComponentPSO].emplace_back(OBB);
		OBB->Position.x += 400.f;

		BoundingSphere* BB = OBB->AddAttachedObjectHelper<BoundingSphere>(GraphicsPipelineCached, AssetManagerCached,
			100.f);

		RendererToObjects[BoundingComponentPSO].emplace_back(OBB);
		RendererToObjects[BoundingComponentPSO].emplace_back(BB);

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
	for (auto& RendererToObjectsPair : RendererToObjects)
	{
		ARenderer* Renderer = RendererToObjectsPair.first;

		Renderer->PresetRendering(CameraCached, this);

		for (const auto& ro : RendererToObjectsPair.second)
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
