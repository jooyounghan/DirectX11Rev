#pragma once
#include "IGuiTopLevelVisitor.h"
#include "IGuiLowLevelVisitor.h"

class MapOutlinerWindow;
class AAttachable;
class APlaceable;

class ModelListOutlinerVisitor : public IGuiTopLevelVisitor, public IGuiLowLevelVisitor
{
public:
	ModelListOutlinerVisitor(MapOutlinerWindow* MapOutlinerInstance);

protected:
	MapOutlinerWindow* MapOutlinerCached;

public:
	virtual void Visit(AttachableObject* AttachableInstance) override;
	virtual void Visit(PlaceableObject* PlaceableInstance) override;
	virtual void Visit(RelativePlaceableObject* RelativePlaceableObject) override;

public:
	void DrawAttachableInOutliner(AAttachable* Attachable);
	void DrawPlaceableInOutliner(APlaceable* Placeable);

};

