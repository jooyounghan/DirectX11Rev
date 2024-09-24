#pragma once
#include "IWindow.h"

class EditorWorld;
class GameWorld;

class MapAsset;

class AObject;
class APlaceableObject;
class AAttachableObject;

class MapOutlinerWindow : public IWindow
{
public:
	MapOutlinerWindow(EditorWorld* EditorWorldIn);
	virtual ~MapOutlinerWindow();

public:
	virtual void RenderWindow() override;

private:
	EditorWorld* EditorWorldCached = nullptr;
	GameWorld* GameWorldCached = nullptr;
	MapAsset* CurrentMap = nullptr;

private:
	ImVec2 RegionAvail = ImVec2();

private:
	void RenderMapAssetOverview();
	void RenderPlaceablesOutline();
	void RenderSelectedPlaceableOutline();

private:
	void DoModalDeleteMap();
	void DoModalDeletePlaceableObject();
	void DoModalDeleteAttachableObject();

private:
	void RenderMapInformation();
	void RenderPlacedListBox();
	void RenderAttachedTree();

private:
	void RenderAttachedOutline(AAttachableObject* Attachment);

private:
	void SetWindowPosToCenter();
	void ResetWindowPosToPrevious();

private:
	ImVec2 PreviousWindowPos = ImVec2();

private:
	static const char* AddMapButtonID;
	static const char* AddPlaceableButtonID;
	static const char* AddAttachableButtonID;

private:
	static const char* DeleteMapModalID;
	static const char* DeletePlaceableModalID;
	static const char* DeleteAttachableModalID;

private:
	static const char* AddMapModalID;
	static const char* AddPlaceableModalID;
	static const char* AddAttachableModalID;

private:
	void DoModalAddNewMap();
	void DoModalAddPlaceableObject();
	void DoModalAddAttachableObject();

};

