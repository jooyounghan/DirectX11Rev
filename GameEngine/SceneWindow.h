#pragma once
#include "AWindow.h"

#include "ComponentPSOManager.h"
#include "Scene.h"
#include "CameraComponent.h"
#include "ComponentInformer.h"

#include "SceneForwardRenderer.h"
#include "SceneDefferedRenderer.h"

#include "ImGuiComboBox.h"

enum class ERendererType
{
	FORWARD_RENDERING,
	DEFFERED_RENDERING
};

class SceneWindow : public AWindow
{
public:
	SceneWindow(
		const std::string& windowID,
		ID3D11DeviceContext** deviceConextAddress,
		AssetManager* assetManager,
		ComponentManager* componentManager, 
		ComponentPSOManager* componentPsoManager
	);

private:
	Scene* m_selectedScene = nullptr;
	ComponentManager* m_componentManagerCached = nullptr;
	ComponentPSOManager* m_componentPsoManageCached = nullptr;

public:
	virtual void PrepareWindow() override;

private:
	virtual void RenderWindowImpl() override;
	void RenderComponentRecursive(ASceneRenderer* const renderer, const std::vector<AComponent*>& components);

private:
	AComponent* m_selectedComponent = nullptr;
	ComponentInformer m_componentInformer;

private:
	SceneForwardRenderer m_forwardRenderer;
	SceneDefferedRenderer m_defferedRenderer;
	ERendererType m_selectedRendererType = ERendererType::FORWARD_RENDERING;

private:
	ImGuiComboBox m_rendererComboBox;

private:
	void DrawSceneSelector();
	void DrawRendererSelector();
	void DrawScene();
	void DrawComponentTree();
	void DrawComponentInformations();

private:
	void DrawComponentTreeRecursive(AComponent* const component);
};

