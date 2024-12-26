#pragma once
#include "AWindow.h"
#include "PSOManager.h"
#include "Scene.h"
#include "SceneRenderer.h"
#include "CameraComponent.h"

constexpr float GSceneNearZ = 0.1f;
constexpr float GSceneFarZ = 1E9f;
constexpr float GSceneFovAngle = 120.f;

class SceneWindow : public AWindow
{
public:
	SceneWindow(const std::string& windowID, PSOManager* psoManager, const uint32_t& width, const uint32_t& height);

private:
	Scene* m_selectedScene = nullptr;
	SceneRenderer m_sceneRenderer;

public:
	virtual void RenderWindowImpl() override;
	virtual void InitializeWindow(ID3D11Device* device, ID3D11DeviceContext* deviceContext) override;

private:
	AComponent* m_selectedComponent = nullptr;

private:
	void RenderScene();
	void RenderComponentTree();
	void RenderComponentHandler();
};

