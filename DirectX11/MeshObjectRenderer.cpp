#include "MeshObjectRenderer.h"
#include "MeshObject.h"
#include "AMeshAsset.h"

#include "PSOObject.h"

#include "Camera.h"
#include "Map.h"

using namespace std;

MeshObjectRenderer::MeshObjectRenderer(PSOObject* PSOObjectIn)
	: ARenderer(PSOObjectIn)
{
}

void MeshObjectRenderer::PresetRendering(Camera* CameraIn, Map* MapIn)
{
	ID3D11RenderTargetView* RTVs[] = { CameraIn->GetSceneRTV() };
	D3D11_VIEWPORT Viewports[] = { CameraIn->GetViewport() };
	ID3D11DepthStencilView* DSV = CameraIn->GetSceneDSV();

	PSOObjectCached->SetPipelineObject(1, RTVs, Viewports, DSV);

	ID3D11Buffer* ViewProjBuffer[] = { CameraIn->ViewProjBuffer.GetBuffer() };
	PSOObjectCached->SetVSConstantBuffers(0, 1, ViewProjBuffer);
}

void MeshObjectRenderer::ResetRendering()
{
	PSOObjectCached->ResetVSConstantBuffers(0, 1);
}

void MeshObjectRenderer::Render(MeshObject* MeshObjectIn)
{
	AMeshAsset* MeshAssetInstance = MeshObjectIn->MeshAssetInstance;
	ID3D11DeviceContext* DeviceContextCached = MeshObjectIn->DeviceContextCached;
	if (MeshAssetInstance)
	{
		const vector<ID3D11Buffer*> VertexBuffers = MeshAssetInstance->GetVertexBuffers();
		const vector<UINT> Strides = MeshAssetInstance->GetStrides();
		const vector<UINT> Offsets = MeshAssetInstance->GetOffsets();

		DeviceContextCached->IASetVertexBuffers(0, VertexBuffers.size(),
			VertexBuffers.data(),
			Strides.data(),
			Offsets.data()
		);
		DeviceContextCached->IASetIndexBuffer(MeshAssetInstance->GetIndexBuffer(), MeshAssetInstance->GetIndexFormat(), 0);

		ID3D11Buffer* VSConstBuffers[] = { MeshObjectIn->TransformationBuffer.GetBuffer() };

		PSOObjectCached->SetVSConstantBuffers(1, 1, VSConstBuffers);

#ifdef _DEBUG
		PSOObjectCached->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextCached->DrawIndexed(static_cast<UINT>(MeshAssetInstance->GetIndexCount()), 0, 0);

		PSOObjectCached->ResetVSConstantBuffers(1, 1);
	}

}

void MeshObjectRenderer::Render(ABoundingComponent* MeshObjectIn)
{
	// Do Nothing
}