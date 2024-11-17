#pragma once
#include "HeaderHelper.h"
#include "AModal.h"
#include <memory>

class MapAsset;

class AddPlaceableModal : public AModal
{
public:
	AddPlaceableModal(const std::string& ModalHeaderNameIn);
	virtual ~AddPlaceableModal();

private:
	std::vector<std::string> PlaceableItemIdentifiers;
	std::shared_ptr<MapAsset> MapAssetCached = nullptr;
	MakeSetterGetter(MapAssetCached);


protected:
	virtual bool ModalCondition() override;
	virtual void RenderModal() override;
};

