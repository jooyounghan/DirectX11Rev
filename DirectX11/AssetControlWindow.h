#pragma once
#include "AWindow.h"
#include "AAssetFile.h"
#include "StaticAssertHelper.h"
#include "UIVariable.h"
#include "AAssetNodeCanvas.h"

#include <string>
#include <memory>
#include <format>

template<typename T, typename ...Args>
class AssetControlWindow : public AWindow
{
public:
	AssetControlWindow(AAssetFile* AssetFileIn, Args... args);

protected:
	std::string WindowDescription;
	std::unique_ptr<AAssetNodeCanvas> NodeCanvasInstance;

protected:
	bool bIsOpen = true;

public:
	virtual void RenderWindow() override;
};

template<typename T, typename ...Args>
inline AssetControlWindow<T, Args...>::AssetControlWindow(AAssetFile* AssetFileIn, Args... args)
{
	static_assert(std::is_base_of<AAssetNodeCanvas, T>::value, DerivedCondition(AAssetNodeCanvas));

	NodeCanvasInstance = std::make_unique<T>(AssetFileIn, args...);
	if (AssetFileIn != nullptr)
	{
		WindowDescription = "Asset Control Window For " + AssetFileIn->GetAssetName();
	}
	else
	{
		WindowDescription = std::string(std::format("Asset Control Window For {}", (uint64_t)AssetFileIn));
	}
}

template<typename T, typename ...Args>
inline void AssetControlWindow<T, Args...>::RenderWindow()
{
	ImGui::SetNextWindowSizeConstraints(NodeCanvasInstance->GetCanvasSize(), ImVec2(FLT_MAX, FLT_MAX));

	ImGui::Begin(
		WindowDescription.c_str(), &bIsOpen,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings
	);


	NodeCanvasInstance->RenderControl();
	ImGui::End();

	if (!bIsOpen)
	{
		return CloseEvent.Invoke(this);
	}
}
