#include "IEditorLinkedWindow.h"
#include "EditorWorld.h"

IEditorLinkedWindow::IEditorLinkedWindow(EditorWorld* EditorWorldIn)
    : EditorWorldCached(EditorWorldIn)
{
    GameWorldCached = EditorWorldCached->GetGameWorldCached();

    assert(EditorWorldCached != nullptr);
    assert(GameWorldCached != nullptr);
}

IEditorLinkedWindow::~IEditorLinkedWindow()
{
}
