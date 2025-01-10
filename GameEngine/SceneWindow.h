#pragma once
#include "AWindow.h"

#include "ComponentInformer.h"
#include "SceneForwardRenderer.h"
#include "SceneDefferedRenderer.h"
#include "ImGuiComboBox.h"

enum class ERendererType
{
	FORWARD_RENDERING,
	DEFFERED_RENDERING
};

class Scene;
class AComponent;
class CameraComponent;

class AssetManager;
class ComponentManager;
class ComponentPSOManager;

struct ID3D11DeviceContext;
struct ID3D11Texture2D;

class SceneWindow : public AWindow
{
public:
	SceneWindow(
		const std::string& windowID,
		ID3D11DeviceContext* const* deviceConextAddress,
		AssetManager* assetManager,
		ComponentManager* componentManager, 
		ComponentPSOManager* componentPsoManager
	);
	~SceneWindow() override = default;

private:
	ID3D11DeviceContext* const* m_deviceContextAddressCached = nullptr;
	Scene* m_selectedScene = nullptr;
	ComponentManager* m_componentManagerCached = nullptr;
	ComponentPSOManager* m_componentPsoManageCached = nullptr;
	CameraComponent* m_selectedCamera = nullptr;


public:
	inline void SetCameraComponent(CameraComponent* cameraComponent) { m_selectedCamera = cameraComponent; }
	virtual void PrepareWindow() override;

private:
	void RenderComponentRecursive(ASceneRenderer* const renderer, const std::vector<AComponent*>& components);

private:
	virtual void RenderWindowImpl() override;

private:
	AComponent* m_selectedComponent = nullptr;
	ComponentInformer m_componentInformer;

private:
	SceneForwardRenderer m_forwardRenderer;
	SceneDefferedRenderer m_defferedRenderer;
	ASceneRenderer* m_selectedRenderer;

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

