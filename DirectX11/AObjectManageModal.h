#pragma once
#include "AModal.h"
#include "HeaderHelper.h"

class EditorWorld;
class MapAsset;

class AObjectManageModal : public AModal
{
public:
	AObjectManageModal(
		const std::string& ModalHeaderNameIn,
		EditorWorld* EditorWorldIn
	);
	virtual ~AObjectManageModal();

protected:
	EditorWorld* EditorWorldCached = nullptr;
	MapAsset* CurrentMapAssetCached = nullptr;
	MakeSetter(CurrentMapAssetCached);

protected:
	virtual bool ModalCondition() = 0;
	virtual void RenderModal() = 0;
};

