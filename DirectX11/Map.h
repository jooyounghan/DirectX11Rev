#pragma once
#include "Serializable.h"
#include "HeaderHelper.h"

#include <list>
#include <memory>
#include <unordered_map>

class GraphicsPipeline;
class PSOManager;
class PSOObject;

class Camera;
class RenderObject;
class IMeshAsset;

class Map : public ISerializable
{
public:
	Map(GraphicsPipeline* GraphicsPipelineInstance, PSOManager* PSOManagerInstance);
	~Map();

protected:
	GraphicsPipeline* GraphicsPipelineCached = nullptr;
	PSOManager* PSOManagerCached = nullptr;

protected:
	/* 
	맵에서 관리하는 객체
	외부 배경 텍스쳐, 몬스터 인스턴스, 접속중인 플레이어 등을 관리	
	*/

protected:
	std::unique_ptr<Camera> MapCamera;
	MakeSmartPtrGetter(MapCamera);

protected:
	std::unordered_map<PSOObject*, std::vector<RenderObject*>> PSOObjectToRenderObjects;

protected:
	std::list<RenderObject> RenderObjects;

public:
	void AddRenderObject(IMeshAsset* MeshAssetIn, float PosXIn, float PosYIn, float PosZIn);
	void AddRenderObject(IMeshAsset* MeshAssetIn, float ScreenXIn, float ScreenYIn, float ScreenWidthIn, float ScreenHeightIn);

public:
	void UpdateMap(const float& DeltaTimeIn);
	void RenderMap();

public:
	virtual void Serialize(const std::string& OutputAdditionalPath) override;
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn) override;
};

