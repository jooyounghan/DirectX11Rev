#pragma once
#include "AContextMenu.h"

class AssetContextMenu : public AContextMenu
{
public:
	AssetContextMenu(const std::string& modalHeaderName);
	~AssetContextMenu() override = default;

protected:
	virtual bool OpenCondition() override;
	virtual void RenderNotification() override;

protected:
	bool m_createIBLMaterialOpenFlag = false;

public:
	bool* GetCreateIBLMaterialOpenFlag() { return &m_createIBLMaterialOpenFlag; }

};

