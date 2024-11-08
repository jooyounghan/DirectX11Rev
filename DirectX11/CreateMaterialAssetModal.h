#pragma once
#include "CreateAssetModal.h"

class CreateMaterialAssetModal : public CreateAssetModal
{
public:
	CreateMaterialAssetModal(const std::string& ModalHeaderNameIn, AssetManager* AssetManagerIn);
	virtual ~CreateMaterialAssetModal();

protected:
	virtual void RenderModal() override;
};

