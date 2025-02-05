#pragma once
#include "AWindow.h"
#include "AnimationRetargeter.h"
#include "ImGuiComboBox.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

class CreateAnimationRetargeterWindow : public AWindow
{
public:
	CreateAnimationRetargeterWindow(
		ID3D11Device* const* deviceAddress, 
		ID3D11DeviceContext* const* deviceContextAddress,
		const std::string& windowID, bool* openFlag
	);
	~CreateAnimationRetargeterWindow() override = default;
	
protected:
	ID3D11Device* const* m_deviceAddress = nullptr;
	ID3D11DeviceContext* const* m_deviceContextAddress = nullptr;

protected:
	AnimationRetargeter m_animationRetargeter;

protected:
	ImGuiComboBox m_boneSourceComboBox;
	ImGuiComboBox m_boneTargetComboBox;
	ImGuiComboBox m_animationSourceComboBox;

protected:
	std::string m_selectedBoneSourceName;
	std::string m_selectedBoneTargetName;
	std::string m_selectedAnimationSourceName;

protected:
	virtual void PrepareWindow() override {};
	virtual void RenderWindowImpl() override;
};

