#pragma once
#include "AModal.h"

class MapOutlinerWindow;

class DeleteAttachableModal : public AModal
{
public:
	DeleteAttachableModal(MapOutlinerWindow* MapOutlinerWindowIn, const std::string& ModalHeaderNameIn);
	virtual ~DeleteAttachableModal();

private:
	MapOutlinerWindow* MapOutlinerWindowCached = nullptr;

protected:
	virtual bool ModalCondition() override;
	virtual void RenderModal() override;
};

