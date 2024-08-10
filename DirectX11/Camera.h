#pragma once

#include "Viewable.h"
#include <memory>

class GraphicsPipeline;

class Camera : public Viewable
{
public: 
	Camera(GraphicsPipeline* GraphicsPipelineInstance, const UINT& WidthIn, const UINT& HeightIn);
	virtual ~Camera();

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				SceneTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	SceneSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		SceneRTV;
	MakeComPtrGetter(SceneTexture2D);
	MakeComPtrGetter(SceneSRV);
	MakeComPtrGetter(SceneRTV);

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				DepthStencilTexture2D;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		SceneDSV;
	MakeComPtrGetter(SceneDSV);

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				ResolvedSceneTexture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	ResolvedSceneSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		ResolvedSceneRTV;
	MakeComPtrGetter(ResolvedSceneTexture2D);
	MakeComPtrGetter(ResolvedSceneSRV);
	MakeComPtrGetter(ResolvedSceneRTV);

protected:
	bool IsMultiSampling = false;

protected:
	const FLOAT ClearColor[4] = { 0.2f, 0.2f, 0.2f, 1.f };

public:
	virtual void CleanupLens();
};

