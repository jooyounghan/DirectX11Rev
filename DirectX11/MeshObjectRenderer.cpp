#include "MeshObjectRenderer.h"
#include "MeshObject.h"
#include "AMeshAsset.h"

#include "PSOObject.h"

#include "Camera.h"
#include "MapAsset.h"

using namespace std;

MeshObjectRenderer::MeshObjectRenderer(PSOObject* PSOObjectIn)
	: ARenderer(PSOObjectIn)
{
}

void MeshObjectRenderer::PresetRendering(const unsigned int& RTVCountIn, ID3D11RenderTargetView** RTVsIn, D3D11_VIEWPORT* ViewportIn, ID3D11DepthStencilView* DSVIn, Camera* CameraIn, MapAsset* MapIn)
{
	PSOObjectCached->SetPipelineObject(RTVCountIn, RTVsIn, ViewportIn, DSVIn);

	ID3D11Buffer* ViewProjBuffer[] = { CameraIn->ViewProjBuffer.GetBuffer() };
	PSOObjectCached->SetVSConstantBuffers(0, 1, ViewProjBuffer);
}

void MeshObjectRenderer::ResetRendering()
{
	PSOObjectCached->ResetVSConstantBuffers(0, 1);
}

void MeshObjectRenderer::Render(ID3D11DeviceContext* DeviceContextIn, MeshObject* MeshObjectIn)
{
	AMeshAsset* MeshAssetInstance = MeshObjectIn->GetMeshAssetInstance();
	if (MeshAssetInstance)
	{
		const vector<ID3D11Buffer*> VertexBuffers = MeshAssetInstance->GetVertexBuffers();
		const vector<UINT> Strides = MeshAssetInstance->GetStrides();
		const vector<UINT> Offsets = MeshAssetInstance->GetOffsets();

		DeviceContextIn->IASetVertexBuffers(0, static_cast<UINT>(VertexBuffers.size()),
			VertexBuffers.data(),
			Strides.data(),
			Offsets.data()
		);
		DeviceContextIn->IASetIndexBuffer(MeshAssetInstance->GetIndexBuffer(), MeshAssetInstance->GetIndexFormat(), 0);

		ID3D11Buffer* VSConstBuffers[] = { MeshObjectIn->TransformationBuffer.GetBuffer() };

		PSOObjectCached->SetVSConstantBuffers(1, 1, VSConstBuffers);

#ifdef _DEBUG
		PSOObjectCached->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextIn->DrawIndexed(static_cast<UINT>(MeshAssetInstance->GetIndexCount()), 0, 0);

		PSOObjectCached->ResetVSConstantBuffers(1, 1);
	}
}

void MeshObjectRenderer::Render(ID3D11DeviceContext* DeviceContextIn, ABoundingComponent* MeshObjectIn)
{
	// Do Nothing
}