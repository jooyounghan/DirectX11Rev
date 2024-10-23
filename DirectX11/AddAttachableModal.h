#pragma once
#include "AObjectManageModal.h"

class AddAttachableModal : public AObjectManageModal
{
public:
	AddAttachableModal(const std::string& ModalHeaderNameIn, EditorWorld* EditorWorldIn);
	virtual ~AddAttachableModal();

private:
	static const char* AttachableItemIdentifiers[];

protected:
	virtual bool ModalCondition() override;
	virtual void RenderModal() override;
};

