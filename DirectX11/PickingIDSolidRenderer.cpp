#include "PickingIDSolidRenderer.h"

#include "MeshObject.h"
#include "AMeshAsset.h"

#include "PSOObject.h"

using namespace std;

PickingIDSolidRenderer::PickingIDSolidRenderer(PSOObject* PSOObjectIn)
	: APickingIDRenderer(PSOObjectIn)
{
}

void PickingIDSolidRenderer::Render(ID3D11DeviceContext* DeviceContextIn, MeshObject* MeshObjectIn)
{

	AMeshAsset* MeshAssetInstance = MeshObjectIn->GetMeshAssetInstance();

	if (MeshAssetInstance)
	{
		const vector<ID3D11Buffer*> VertexBuffers = MeshAssetInstance->GetPositionBuffer();
		const vector<UINT> Strides = MeshAssetInstance->GetPositionStride();
		const vector<UINT> Offsets = MeshAssetInstance->GetPositionOffset();

		DeviceContextIn->IASetVertexBuffers(0, static_cast<UINT>(VertexBuffers.size()),
			VertexBuffers.data(),
			Strides.data(),
			Offsets.data()
		);
		DeviceContextIn->IASetIndexBuffer(MeshAssetInstance->GetIndexBuffer(), MeshAssetInstance->GetIndexFormat(), 0);

		ID3D11Buffer* VSConstBuffers[] = { MeshObjectIn->TransformationBuffer.GetBuffer() };
		ID3D11Buffer* PSConstBuffers[] = { MeshObjectIn->GetPickingIDBuffer().GetBuffer() };

		PSOObjectCached->SetVSConstantBuffers(1, 1, VSConstBuffers);
		PSOObjectCached->SetPSConstantBuffers(0, 1, PSConstBuffers);
	#ifdef _DEBUG
		PSOObjectCached->CheckPipelineValidation();
	#endif // DEBUG

		DeviceContextIn->DrawIndexed(static_cast<UINT>(MeshAssetInstance->GetIndexCount()), 0, 0);

		PSOObjectCached->ResetVSConstantBuffers(1, 1);
	}
}

void PickingIDSolidRenderer::Render(ID3D11DeviceContext* DeviceContextIn, ABoundingComponent* MeshObjectIn)
{
}
