#pragma once
#include "AWindow.h"
#include "AddAttachableModal.h"
#include "AddPlaceableModal.h"
#include "DeleteAttachableModal.h"
#include "DeletePlaceableModal.h"

class EditorWorld;
class GameWorld;
class MapAsset;

class AAttachableObject;

class MapOutlinerWindow : public AWindow
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

protected:
	AddAttachableModal AddAttachableModalInstance;
	AddPlaceableModal AddPlaceableModalInstance;
	DeleteAttachableModal DeleteAttachableModalInstance;
	DeletePlaceableModal DeletePlaceableModalInstance;

private:
	ImVec2 RegionAvail = ImVec2();

private:
	void RenderMapAssetOverview();
	void RenderPlaceablesOutline();
	void RenderSelectedPlaceableOutline();

private:
	void RenderMapInformation();
	void RenderPlacedListBox();
	void RenderAttachedTree();

private:
	void RenderAttachedOutline(AAttachableObject* Attachment);

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

};

