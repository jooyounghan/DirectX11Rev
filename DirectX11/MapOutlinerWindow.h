#pragma once
#include "IEditorLinkedWindow.h"

class AObject;

class MapOutlinerWindow : public IEditorLinkedWindow
{
public:
	MapOutlinerWindow(EditorWorld* EditorWorldIn);
	virtual ~MapOutlinerWindow();

public:
	virtual void RenderWindow() override;

private:
	void RenderPlaceablesOutline();
	void RenderSelectedObjectInformation();
};

