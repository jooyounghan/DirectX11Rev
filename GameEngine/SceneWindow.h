#pragma once
#include "AWindow.h"

#include "ComponentHandler.h"
#include "SceneForwardRenderer.h"
#include "SceneDeferredRenderer.h"
#include "ImGuiComboBox.h"

enum class ERendererType
{
	FORWARD_RENDERING,
	Deferred_RENDERING
};

class Scene;
class AComponent;
class CameraComponent;

class ComponentManager;
class ComponentPSOManager;

struct ID3D11DeviceContext;
struct ID3D11Texture2D;

class SceneWindow : public AWindow
{
public:
	SceneWindow(
		const std::string& windowID,
		ID3D11Device* device,
		ID3D11DeviceContext* immediateContext,
		ID3D11DeviceContext* componentRenderDeferredContext,
		ComponentManager* componentManager, 
		ComponentPSOManager* componentPsoManager
	);
	~SceneWindow() override = default;

private:
	ID3D11DeviceContext* m_immediateContext = nullptr;
	ID3D11DeviceContext* m_componentRenderDeferredContext = nullptr;
	Scene* m_selectedScene = nullptr;
	ComponentManager* m_componentManagerCached = nullptr;
	ComponentPSOManager* m_componentPsoManageCached = nullptr;
	CameraComponent* m_selectedCamera = nullptr;

public:
	inline void SetCameraComponent(CameraComponent* cameraComponent) { m_selectedCamera = cameraComponent; }
	virtual void PrepareWindow() override;

protected:
	virtual void RenderWindowImpl() override;

private:
	AComponent* m_selectedComponent = nullptr;
	ComponentHandler m_componentHandler;

private:
	SceneForwardRenderer m_forwardRenderer;
	SceneDeferredRenderer m_DeferredRenderer;
	ASceneRenderer* m_selectedRenderer = nullptr;
	bool m_isDeferredRenderer = false;

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
	void InteractSceneInput(const ImVec2& size);
	uint32_t GetMouseClickedComponentID();
};

