#pragma once
#include "Serializable.h"

#include <vector>
#include <memory>

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

protected:
	std::vector<std::unique_ptr<RenderObject>> RenderObjects;

public:
	void UpdateMap(const float& DeltaTimeIn);
	void RenderMap(Camera* CameraIn);

public:
	virtual void Serialize(const std::string& OutputAdditionalPath) override;
	virtual void Deserialize(const std::string& InputFullPath, ID3D11Device* DeviceIn) override;
};

