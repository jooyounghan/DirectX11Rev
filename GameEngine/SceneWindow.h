#pragma once
#include "AWindow.h"

#include "PSOManager.h"
#include "Scene.h"
#include "SceneRenderer.h"
#include "CameraComponent.h"
#include "ComponentInformer.h"

//constexpr float GSceneNearZ = 0.1f;
//constexpr float GSceneFarZ = 1E9f;
//constexpr float GSceneFovAngle = 120.f;

class SceneWindow : public AWindow
{
public:
	SceneWindow(
		const std::string& windowID, 
		ComponentManager* componentManager, 
		PSOManager* psoManager
	);

private:
	Scene* m_selectedScene = nullptr;

private:

private:
	ComponentManager* m_componentManagerCached = nullptr;
	PSOManager* m_psoManageCached = nullptr;

public:
	virtual void RenderWindowImpl() override;
	virtual void InitializeWindow(ID3D11Device* device, ID3D11DeviceContext* deviceContext) override;

private:
	AComponent* m_selectedComponent = nullptr;
	ComponentInformer m_componentInformer;

private:
	void RenderSceneSelector();
	void RenderRendererSelector();
	void RenderScene();
	void RenderComponentTree();
	void RenderComponentInformations();

private:
	void RenderComponentRecursive(AComponent* const component);
};

