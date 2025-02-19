#pragma once
#include "AComponent.h"

class ModelMaterialAsset;
class IModelMaterialProvider;

class AMeshComponent : public AComponent
{
public:
	AMeshComponent(
		const std::string& componentName,
		const uint32_t& componentID, 
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale	
	);
	~AMeshComponent() override = default;

protected:
	std::vector<std::string> m_modelMaterialNames;
	std::vector<ModelMaterialAsset*> m_selectedModelMaterials;

public:
	inline void SetModelMaterialName(const std::vector<std::string>& modelMaterialNames) { m_modelMaterialNames = modelMaterialNames; }
	inline const std::vector<std::string>& GetModelMaterialName() const { return m_modelMaterialNames; }

public:
	inline const std::vector<ModelMaterialAsset*>& GetSelectedModelMaterials() const { return m_selectedModelMaterials; }

public:
	void UpdateModelMaterial(IModelMaterialProvider& provider);
};

