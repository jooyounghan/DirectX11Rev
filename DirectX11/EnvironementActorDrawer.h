#pragma once
#include "IInformationDrawer.h"

class EnvironmentActor;
class AssetManager;

class EnvironementActorDrawer : AInformationDrawer<EnvironmentActor>
{
public: EnvironementActorDrawer(EnvironmentActor* EnvironmentActorIn, AssetManager* AssetManagerIn);;

protected:
	AssetManager* AssetManagerCached = nullptr;

public:
	virtual void DrawInformation() override;
};

