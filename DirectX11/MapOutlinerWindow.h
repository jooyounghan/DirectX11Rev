#pragma once
#include "AWindow.h"

#include "AddAttachableModal.h"
#include "AddPlaceableModal.h"
#include "DeleteAttachableModal.h"
#include "DeletePlaceableModal.h"

#include <memory>

class GameWorld;
class MapAsset;
class APlaceableObject;
class AAttachableObject;

typedef std::function<void(const std::shared_ptr<MapAsset>&)> MapSelectedHandler;

class MapOutlinerWindow : public AWindow
{
public:
	MapOutlinerWindow();
	virtual ~MapOutlinerWindow();

public:
	virtual void RenderWindow() override;

private:
	void RenderMapOutliner();
	void RenderModelDetail();

public:
	Delegation<const std::shared_ptr<MapAsset>&> MapSelectedEvent;

public:
	std::shared_ptr<MapAsset> SelectedMapAsset = nullptr;

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

