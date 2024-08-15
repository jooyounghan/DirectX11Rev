#pragma once
#include "IWindow.h"
#include "HeaderHelper.h"
#include <string>

class EditorWorld;
class GameWorld;

class IEditorLinkedWindow : public IWindow
{
public:
	IEditorLinkedWindow(EditorWorld* EditorWorldIn);
	virtual ~IEditorLinkedWindow();

protected:
	EditorWorld* EditorWorldCached= nullptr;
	GameWorld* GameWorldCached = nullptr;
	MakeGetter(EditorWorldCached);
	MakeGetter(GameWorldCached);
};

