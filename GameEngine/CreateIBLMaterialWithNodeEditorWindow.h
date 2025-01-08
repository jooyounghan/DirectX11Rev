#pragma once
#include "ANodeEditorWindow.h"

class AssetManager;

class CreateIBLMaterialWithNodeEditorWindow : public ANodeEditorWindow
{
public:
	CreateIBLMaterialWithNodeEditorWindow(
		ID3D11Device* const* deviceAddress, ID3D11DeviceContext* const* deviceContextAddress,
		AssetManager* assetManager, const std::string& windowID, bool* openFlag
	);
	~CreateIBLMaterialWithNodeEditorWindow() override = default;

protected:
	AssetManager* m_assetManagerCached = nullptr;

private:
	virtual void RenderWindowImpl() override;
};

