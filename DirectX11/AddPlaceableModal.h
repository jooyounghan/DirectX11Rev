#pragma once
#include "AObjectManageModal.h"

class AddPlaceableModal : public AObjectManageModal
{
public:
	AddPlaceableModal(const std::string& ModalHeaderNameIn, EditorWorld* EditorWorldIn);
	virtual ~AddPlaceableModal();

protected:
	virtual bool ModalCondition() override;
	virtual void RenderModal() override;
};

