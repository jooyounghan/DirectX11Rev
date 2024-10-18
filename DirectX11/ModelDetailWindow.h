#pragma once
#include "AWindow.h"

class EditorWorld;
class AssetManager;

class ModelDetailWindow : public AWindow
{
public:
	ModelDetailWindow(EditorWorld* EditorWorldIn);
	virtual ~ModelDetailWindow();

protected:
	EditorWorld* EditorWorldCached = nullptr;
	AssetManager* AssetManagerCached = nullptr;

public:
	virtual void RenderWindow() override;

private:
	void DrawDetailProperties();
};

