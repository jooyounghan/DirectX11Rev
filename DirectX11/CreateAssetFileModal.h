#pragma once
#include "AModal.h"
#include "HeaderHelper.h"

class AssetManager;

class CreateAssetFileModal : public AModal
{
public:
	CreateAssetFileModal(const std::string& ModalHeaderNameIn, AssetManager* AssetManagerIn);
	virtual ~CreateAssetFileModal();

protected:
	AssetManager* AssetManagerCached = nullptr;

protected:
	bool ModalFlag = false;
	MakeSetterGetter(ModalFlag);

protected:
	virtual bool ModalCondition() override;
	virtual void RenderModal() override;
};

