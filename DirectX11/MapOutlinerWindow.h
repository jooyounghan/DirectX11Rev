#pragma once
#include "AWindow.h"

#include "HeaderHelper.h"
#include "AddAttachableModal.h"
#include "AddPlaceableModal.h"
#include "DeleteAttachableModal.h"
#include "DeletePlaceableModal.h"

#include <memory>

struct Ray;
struct ImVec2;

class AAssetFile;
class MapAsset;
class APlaceableObject;
class AAttachableObject;
class ACamera;

typedef std::function<void(const unsigned int&)> IDSelectHandler;
typedef std::function<void(AAssetFile*, const float&, const float&, const float&)> AssetDropHandler;
typedef std::function<void(const std::shared_ptr<MapAsset>&)> MapSelectedHandler;

class MapOutlinerWindow : public AWindow
{
public:
	MapOutlinerWindow();
	virtual ~MapOutlinerWindow();

public:
	virtual void RenderWindow() override;

private:
	void RenderViewport();
	void RenderMapOutliner();
	void RenderModelDetail();

public:
	Delegation<const unsigned int&> IDSelectEvent;
	Delegation<AAssetFile*, const float&, const float&, const float&> AssetDropEvent;
	Delegation<const std::shared_ptr<MapAsset>&> MapSelectedEvent;

protected:
	std::shared_ptr<MapAsset> SelectedMapAsset = nullptr; 
	ACamera* CurrentCamera = nullptr;
	MakeSetter(CurrentCamera);

protected:
	AddAttachableModal AddAttachableModalInstance;
	AddPlaceableModal AddPlaceableModalInstance;
	DeleteAttachableModal DeleteAttachableModalInstance;
	DeletePlaceableModal DeletePlaceableModalInstance;

private:
	void RenderMapAssetOverview();
	void RenderPlaceablesOutline();
	void RenderSelectedPlaceableOutline();

private:
	void RenderAttachedOutline(AAttachableObject* Attachment);

protected:
	APlaceableObject* SelectedPlaceable = nullptr;
	AAttachableObject* SelectedAttachable = nullptr;
	MakeSetterGetter(SelectedPlaceable);
	MakeSetterGetter(SelectedAttachable);

private:
	ImVec2 RegionAvail = ImVec2();
	ImVec2 PreviousWindowPos = ImVec2();
	ImVec2 ImagePosition = ImVec2();
	ImVec2 ImageSize = ImVec2();

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

