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
	�ʿ��� �����ϴ� ��ü
	�ܺ� ��� �ؽ���, ���� �ν��Ͻ�, �������� �÷��̾� ���� ����	
	*/

	// TODO : ���� �Ŵ����� EditWorld�� ����, Map�� RenderObjet�� �߰��ϴ� �Լ��� ����
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

