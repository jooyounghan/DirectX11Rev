#pragma once
#include "IDialog.h"

class FileSelectDialog : public IDialog
{
public:
	FileSelectDialog();
	~FileSelectDialog();

public:
	virtual void DoModal() override;
};

