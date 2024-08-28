#pragma once
#include "IWindow.h"

class EditorWorld;
class GameWorld;

class MapAsset;

class AObject;
class PlaceableObject;
class AttachableObject;

class ModelOutlinerWindow : public IWindow
{
public:
	ModelOutlinerWindow(EditorWorld* EditorWorldIn);
	virtual ~ModelOutlinerWindow();

public:
	virtual void RenderWindow() override;

private:
	EditorWorld* EditorWorldCached = nullptr;
	GameWorld* GameWorldCached = nullptr;
	MapAsset* CurrentMap = nullptr;

private:
	ImVec2 RegionAvail = ImVec2();

private:
	void RenderPlaceablesOutline();
	void RenderSelectedPlaceableOutline();

private:
	void DoModalDeletePlaceableObject();
	void DoModalDeleteAttachableObject();

private:
	void RenderPlacedListBox();
	void RenderAttachedTree();

private:
	void RenderAttachedOutline(AttachableObject* Attachment);

private:
	void SetWindowPosToCenter();
	void ResetWindowPosToPrevious();

private:
	ImVec2 PreviousWindowPos = ImVec2();

private:
	static const char* AddPlaceableButtonID;
	static const char* AddAttachableButtonID;

private:
	static const char* DeletePlaceableModalID;
	static const char* DeleteAttachableModalID;

private:
	static const char* AddPlaceableModalID;
	static const char* AddAttachableModalID;

private:
	void DoModalAddPlaceableObject();
	void DoModalAddAttachableObject();
};

