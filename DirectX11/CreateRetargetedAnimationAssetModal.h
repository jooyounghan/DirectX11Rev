#pragma once
#include "CreateAssetModal.h"
#include "AnimationRetargeter.h"

class CreateRetargetedAnimationAssetModal : public CreateAssetModal
{
public:
	CreateRetargetedAnimationAssetModal(const std::string& ModalHeaderNameIn, AssetManager* AssetManagerIn);
	virtual ~CreateRetargetedAnimationAssetModal();

protected:
	AnimationRetargeter AnimationRetargeterInstance;

protected:
	virtual void RenderModal() override;
};

