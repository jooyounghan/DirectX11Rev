#pragma once
#include "AObjectManageModal.h"

class AddPlaceableModal : public AObjectManageModal
{
public:
	AddPlaceableModal(const std::string& ModalHeaderNameIn, EditorWorld* EditorWorldIn);
	virtual ~AddPlaceableModal();

private:
	static const char* PlaceableItemIdentifiers[];

protected:
	virtual bool ModalCondition() override;
	virtual void RenderModal() override;
};

