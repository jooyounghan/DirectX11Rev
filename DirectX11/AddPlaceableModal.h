#pragma once
#include "AObjectManageModal.h"

class AddPlaceableModal : public AObjectManageModal
{
public:
	AddPlaceableModal(const std::string& ModalHeaderNameIn);
	virtual ~AddPlaceableModal();

private:
	std::vector<std::string> PlaceableItemIdentifiers;

protected:
	virtual bool ModalCondition() override;
	virtual void RenderModal() override;
};

