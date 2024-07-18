#pragma once
#include <string>

class GraphicsPipeline;

class ISerializable
{
public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") = 0;
	virtual void Deserialize(const std::string& InputFullPath, GraphicsPipeline* GraphicsPipelineInstance) = 0;
};

