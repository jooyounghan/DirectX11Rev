#pragma once
#include "ANodeEditorWindow.h"

class CreateIBLMaterialWithNodeEditorWindow : public ANodeEditorWindow
{
public:
	CreateIBLMaterialWithNodeEditorWindow(const std::string& windowID, bool* openFlag);
	~CreateIBLMaterialWithNodeEditorWindow() override = default;

public:
	virtual void PrepareWindow() override {};

private:
	virtual void RenderWindowImpl() override;
};

