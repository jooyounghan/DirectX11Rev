#pragma once
#include "AObjectManageModal.h"

class DeleteAttachableModal : public AObjectManageModal
{
public:
	DeleteAttachableModal(const std::string& ModalHeaderNameIn);
	virtual ~DeleteAttachableModal();

protected:
	virtual bool ModalCondition() override;
	virtual void RenderModal() override;
};

