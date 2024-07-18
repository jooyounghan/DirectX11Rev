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
		RenderObjects.emplace_back(make_unique<RenderObject>(GraphicsPipelineInstance, (IMeshAsset*)FileManagerInstance.GetAsset("Zombie_SkeletalMesh")));
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
	PSOObject* SkeletalSolidPSO = PSOManagerCached->GetPSOObject(PSOType::R8G8B8A8_Skeletal_Wireframe);
	ID3D11RenderTargetView* RTVs[] = { CameraIn->GetSceneRTV() };
	D3D11_VIEWPORT Viewports[] = { CameraIn->GetViewport() };
	ID3D11DepthStencilView* DSV = CameraIn->GetSceneDSV();

	SkeletalSolidPSO->SetPipelineObject(1, RTVs, Viewports, DSV);
	
	CameraIn->CleanupLens();

	ID3D11Buffer* ViewProjBuffer[] = { CameraIn->ViewProjBuffer.GetBuffer() };
	SkeletalSolidPSO->SetVSConstantBuffers(0, 1, ViewProjBuffer);
	
	for (const auto& ro : RenderObjects)
	{
        ro->Render(SkeletalSolidPSO);
	}
	SkeletalSolidPSO->ResetVSConstantBuffers(0, 1);
}

void Map::Serialize(const std::string& OutputAdditionalPath)
{
}

void Map::Deserialize(const std::string& InputFullPath, ID3D11Device* DeviceIn)
{
}
