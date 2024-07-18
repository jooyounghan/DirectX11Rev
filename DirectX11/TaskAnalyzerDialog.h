#pragma once
#include "IDialog.h"

class TaskAnalyzerDialog : public IDialog
{
public:
	TaskAnalyzerDialog();
	virtual ~TaskAnalyzerDialog();

public:
	virtual void DoModal() override;
};

