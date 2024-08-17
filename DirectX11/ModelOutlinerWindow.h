#pragma once
#include "IEditorLinkedWindow.h"

class AObject;
class AttachableObject;
class Map;

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
	void IfDeletePlaceableObject();
	void IfDeleteAttachedObject();

private:
	void RenderPlacedListBox(Map* CurrentMap);
	void RenderAttachedTree();

private:
	void RenderAttachedOutline(AttachableObject* Attachment);

private:
	static const char* AddPlaceableButtonID;
	static const char* AddAttachableButtonID;
	static const char* AddPlaceableModalID;
	static const char* AddAttachableModalID;

private:
	void DoModalAddPlaceableObject();
	void DoModalAddAttachableObject();



};

