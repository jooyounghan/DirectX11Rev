#pragma once
#include "IWindow.h"

class EditorWorld;
class AssetManager;

class ModelDetailWindow : public IWindow
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

