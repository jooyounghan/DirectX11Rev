#pragma once
#include "AObjectManageModal.h"

class AddAttachableModal : public AObjectManageModal
{
public:
	AddAttachableModal(const std::string& ModalHeaderNameIn);
	virtual ~AddAttachableModal();

private:
	std::vector<std::string> AttachableItemIdentifiers;

protected:
	virtual bool ModalCondition() override;
	virtual void RenderModal() override;
};

