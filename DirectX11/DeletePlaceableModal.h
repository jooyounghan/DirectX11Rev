#pragma once
#include "AObjectManageModal.h"

class DeletePlaceableModal : public AObjectManageModal
{
public:
	DeletePlaceableModal(const std::string& ModalHeaderNameIn);
	virtual ~DeletePlaceableModal();

protected:
	virtual bool ModalCondition() override;
	virtual void RenderModal() override;
};

