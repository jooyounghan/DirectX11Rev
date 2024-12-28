#pragma once
#include "AComponent.h"
#include "ModelMaterialAsset.h"

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

protected:
	std::vector<std::string> m_modelMaterialNames;
	std::vector<ModelMaterialAsset*> m_selectedModelMaterials;

public:
	inline void SetModelMaterialName(const std::vector<std::string>& modelMaterialNames) { m_modelMaterialNames = modelMaterialNames; }
	inline const std::vector<std::string>& GetModelMaterialName() { return m_modelMaterialNames; }

public:
	void UpdateModelMaterial(IModelMaterialProvider& provider);
};

