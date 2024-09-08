#include "APickingIDRenderer.h"

#include "PSOObject.h"

#include "Camera.h"
#include "MapAsset.h"

using namespace std;

APickingIDRenderer::APickingIDRenderer(PSOObject* PSOObjectIn)
	: ARenderer(PSOObjectIn)
{
}

void APickingIDRenderer::PresetRendering(
	const unsigned int& RTVCountIn, 
	ID3D11RenderTargetView** RTVsIn, 
	D3D11_VIEWPORT* ViewportIn, 
	ID3D11DepthStencilView* DSVIn, 
	Camera* CameraIn, 
	MapAsset* MapIn
)
{
	PSOObjectCached->SetPipelineStateObject(RTVCountIn, RTVsIn, ViewportIn, DSVIn);

	ID3D11Buffer* ViewProjBuffer[] = { CameraIn->ViewProjBuffer.GetBuffer() };
	PSOObjectCached->SetVSConstantBuffers(0, 1, ViewProjBuffer);
}

void APickingIDRenderer::ResetRendering()
{
	PSOObjectCached->ResetVSConstantBuffers(0, 1);
}