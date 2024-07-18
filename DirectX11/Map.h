#pragma once
#include "Serializable.h"

#include <vector>
#include <memory>

#include "FileManager.h"

class GraphicsPipeline;
class PSOManager;

class Camera;
class RenderObject;

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

	// TODO : 파일 매니저는 EditWorld로 가고, Map에 RenderObjet를 추가하는 함수로 변경
	FileManager FileManagerInstance;

protected:
	std::vector<std::unique_ptr<RenderObject>> RenderObjects;

public:
	void UpdateMap(const float& DeltaTimeIn);
	void RenderMap(Camera* CameraIn);

public:
	virtual void Serialize(const std::string& OutputAdditionalPath) override;
	virtual void Deserialize(const std::string& InputFullPath, GraphicsPipeline* GraphicsPipelineInstance) override;
};

