#pragma once
#include "ANodeEditorWindow.h"

class CreateModelMaterialWIthNodeEditorWindow : public ANodeEditorWindow
{
public:
	CreateModelMaterialWIthNodeEditorWindow(
		ID3D11Device* const* deviceAddress, ID3D11DeviceContext* const* deviceContextAddress,
		const std::string& windowID, bool* openFlag
	);
	~CreateModelMaterialWIthNodeEditorWindow() override = default;

private:
	virtual void RenderWindowImpl() override;
};

