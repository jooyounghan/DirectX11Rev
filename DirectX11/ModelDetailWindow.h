#pragma once
#include "IEditorLinkedWindow.h"

class ModelDetailWindow : public IEditorLinkedWindow
{
public:
	ModelDetailWindow(EditorWorld* EditorWorldIn);
	virtual ~ModelDetailWindow();

public:
	virtual void RenderWindow() override;

private:
	void DrawDetailProperties();
};

