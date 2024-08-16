#pragma once
#include "IEditorLinkedWindow.h"

class AObject;
class AttachableObject;

class ModelOutlinerWindow : public IEditorLinkedWindow
{
public:
	ModelOutlinerWindow(EditorWorld* EditorWorldIn);
	virtual ~ModelOutlinerWindow();

public:
	virtual void RenderWindow() override;

private:
	ImVec2 RegionAvail = ImVec2();

private:
	void RenderPlaceablesOutline();
	void RenderSelectedPlaceableOutline();

private:
	void RenderAttachedOutline(AttachableObject* Attachment);
};

