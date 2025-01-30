#pragma once
#include "ANodeEditorWindow.h"

class CreateIBLMaterialWithNodeEditorWindow : public ANodeEditorWindow
{
public:
	CreateIBLMaterialWithNodeEditorWindow(
		ID3D11Device* const* deviceAddress, ID3D11DeviceContext* const* deviceContextAddress,
		const std::string& windowID, bool* openFlag
	);
	~CreateIBLMaterialWithNodeEditorWindow() override = default;

private:
	virtual void RenderWindowImpl() override;
};

