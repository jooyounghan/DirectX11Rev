#pragma once
#include "AModal.h"
#include "HeaderHelper.h"
#include <memory>

class MapAsset;
class MapOutlinerWindow;

class DeletePlaceableModal : public AModal
{
public:
	DeletePlaceableModal(MapOutlinerWindow* MapOutlinerWindowIn, const std::string& ModalHeaderNameIn);
	virtual ~DeletePlaceableModal();

private:
	MapOutlinerWindow* MapOutlinerWindowCached = nullptr;

private:
	std::shared_ptr<MapAsset> MapAssetCached = nullptr;
	MakeSetterGetter(MapAssetCached);

protected:
	virtual bool ModalCondition() override;
	virtual void RenderModal() override;
};

