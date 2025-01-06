#pragma once
#include "ANodeEditorWindow.h"

class AssetManager;

class CreateIBLMaterialWithNodeEditorWindow : public ANodeEditorWindow
{
public:
	CreateIBLMaterialWithNodeEditorWindow(AssetManager* assetManager, const std::string& windowID, bool* openFlag);
	~CreateIBLMaterialWithNodeEditorWindow() override = default;

protected:
	AssetManager* m_assetManagerCached = nullptr;

private:
	virtual void RenderWindowImpl() override;
};

