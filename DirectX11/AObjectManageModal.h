#pragma once
#include "AModal.h"
#include "HeaderHelper.h"

#include <memory>

class MapAsset;

class AObjectManageModal : public AModal
{
public:
	AObjectManageModal(const std::string& ModalHeaderNameIn);
	virtual ~AObjectManageModal();

protected:
	std::shared_ptr<MapAsset> CurrentMapAssetCached = nullptr;
	MakeSetter(CurrentMapAssetCached);

protected:
	virtual bool ModalCondition() = 0;
	virtual void RenderModal() = 0;
};

