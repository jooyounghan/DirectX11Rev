#include "RenderObject.h"
#include "GraphicsPipeline.h"
#include "IMeshAsset.h"
#include "PSOObject.h"

RenderObject::RenderObject(
	GraphicsPipeline* GraphicsPipelineInstances, 
	IMeshAsset* MeshAssetInstanceIn
)
	: Object(GraphicsPipelineInstances->GetDevice(),
		GraphicsPipelineInstances->GetDeviceContext()),
		MeshAssetInstance(MeshAssetInstanceIn)
{
}

RenderObject::~RenderObject()
{
}

void RenderObject::Render(PSOObject* PSOObjectIn)
{
	if (MeshAssetInstance)
	{
		ID3D11Buffer* VertexBuffers[] = { MeshAssetInstance->GetVertexBuffer() };
		UINT Strides[] = { MeshAssetInstance->GetVertexTypeSize() };
		UINT Offsets[] = { 0 };

		DeviceContextCached->IASetVertexBuffers(0, 1, VertexBuffers, Strides, Offsets);
		DeviceContextCached->IASetIndexBuffer(MeshAssetInstance->GetIndexBuffer(), MeshAssetInstance->GetIndexFormat(), 0);

		ID3D11Buffer* VSConstBuffers[] = { TransformationBuffer.GetBuffer() };
		PSOObjectIn->SetVSConstantBuffers(1, 1, VSConstBuffers);

#ifdef _DEBUG
		PSOObjectIn->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextCached->DrawIndexed(static_cast<UINT>(MeshAssetInstance->GetIndexCount()), 0, 0);

		PSOObjectIn->ResetVSConstantBuffers(1, 0);
	}
}
