#pragma once
#include "IGuiVisitor.h"

class AAttachable;
class APlaceable;
class MapOutlinerWindow;

class OutlinerVisitor : public IGuiVisitor
{
public:
	OutlinerVisitor(MapOutlinerWindow* MapOutlinerInstance);

private:
	MapOutlinerWindow* MapOutlinerCached = nullptr;

public:
	virtual void Visit(AttachableObject* AttachableInstance) override;
	virtual void Visit(PlaceableObject* PlaceableInstance) override;
	virtual void Visit(RelativePlaceableObject* RelativePlaceableObject) override;

public:
	void RenderAttachableInOutliner(AAttachable* Attachable);
	void RenderPlaceableInOutliner(APlaceable* Placeable);

};

