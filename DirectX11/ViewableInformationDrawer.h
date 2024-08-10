#pragma once
#include "IInformationDrawer.h"

class Viewable;

class ViewableInformationDrawer : AInformationDrawer<Viewable>
{
public:
	ViewableInformationDrawer(Viewable* ViewableIn)
		: AInformationDrawer(ViewableIn) {};

public:
	virtual void DrawInformation() override;
};

