#include "Map.h"

#include "PSOManager.h"
#include "PSOObject.h"
#include "RenderObject.h"
#include "Camera.h"

// TODO : FileManager 구조 변경시 삭제
#include "GraphicsPipeline.h"

using namespace std;

Map::Map(GraphicsPipeline* GraphicsPipelineInstance, PSOManager* PSOManagerInstance)
	: GraphicsPipelineCached(GraphicsPipelineInstance), PSOManagerCached(PSOManagerInstance), FileManagerInstance(GraphicsPipelineInstance->GetDevice())
{
	FileManagerInstance.LoadModelFile("./Zombie.fbx");
	for (size_t idx = 0; idx < 1000; ++idx)
	{
		RenderObjects.emplace_back(make_unique<RenderObject>((IMeshAsset*)FileManagerInstance.GetAsset("Zombie_SkeletalMesh"), GraphicsPipelineInstance));
		RenderObjects[RenderObjects.size() - 1]->SetAngle(idx / 10.f, idx / 10.f, idx / 10.f);
	}
}

Map::~Map()
{
}

void Map::UpdateMap(const float& DeltaTimeIn)
{
	for (const auto& ro : RenderObjects)
	{
		ro->UpdateObject();
	}
}

void Map::RenderMap(Camera* CameraIn)
{
	PSOObject* SkeletalSolidPSO = PSOManagerCached->GetPSOObject(PSOType::R8G8B8A8_Skeletal_Solid);
	ID3D11RenderTargetView* RTVs[] = { CameraIn->GetSceneRTV() };
	ID3D11DepthStencilView* DSV = CameraIn->GetSceneDSV();

	SkeletalSolidPSO->SetPipelineObject(1, RTVs, DSV);

	ID3D11Buffer* ViewProjBuffer[] = { CameraIn->ViewProjBuffer.GetBuffer() };
	
	SkeletalSolidPSO->SetVSConstantBuffers(0, 1, ViewProjBuffer);
	for (const auto& ro : RenderObjects)
	{
        ro->Render(RenderingCommandList, CameraIn);
	}

	CameraIn->ResetRenderTarget(RenderingCommandList);
}

void Map::Serialize(const std::string& OutputAdditionalPath)
{
}

void Map::Deserialize(const std::string& InputFullPath, GraphicsPipeline* GraphicsPipelineInstance)
{
}

