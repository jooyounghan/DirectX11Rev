#pragma once
#include "IGameWorldLinkedWindow.h"
#include "HeaderHelper.h"

class AObject;

class MapOutlinerWindow : public IGameWorldLinkedWindow
{
public:
	MapOutlinerWindow(GameWorld* GameWorldLinkedIn);
	virtual ~MapOutlinerWindow();

public:
	virtual void RenderWindow() override;

private:
	AObject* SelectedObject = nullptr;
	MakeSetterGetter(SelectedObject);

private:
	void RenderPlaceablesOutline();
	void RenderSelectedObjectInformation();
};

