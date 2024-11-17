#pragma once
#include "AModal.h"

class MapOutlinerWindow;

class AddAttachableModal : public AModal
{
public:
	AddAttachableModal(MapOutlinerWindow* MapOutlinerWindowIn, const std::string& ModalHeaderNameIn);
	virtual ~AddAttachableModal();

private:
	std::vector<std::string> AttachableItemIdentifiers;

private:
	MapOutlinerWindow* MapOutlinerWindowCached = nullptr;

protected:
	virtual bool ModalCondition() override;
	virtual void RenderModal() override;
};

