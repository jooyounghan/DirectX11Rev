#include "MeshObject.h"
#include "GraphicsPipeline.h"
#include "IMeshAsset.h"
#include "PSOObject.h"

MeshObject::MeshObject(
	GraphicsPipeline* GraphicsPipelineInstances, 
	IMeshAsset* MeshAssetInstanceIn
)
	: RelativePlaceableObject(
		GraphicsPipelineInstances->GetDevice(),
		GraphicsPipelineInstances->GetDeviceContext()
	), MeshAssetInstance(MeshAssetInstanceIn)
{
}

MeshObject::~MeshObject()
{
}

void MeshObject::UpdateObject(const float& DeltaTimeIn)
{
	Position.y += 100.f * DeltaTimeIn;
	Angle.Pitch += 30.f * DeltaTimeIn;
	Angle.Yaw += 30.f * DeltaTimeIn;
	RelativePlaceableObject::UpdateObject(DeltaTimeIn);
}

void MeshObject::Render(PSOObject* PSOObjectIn)
{
	if (MeshAssetInstance)
	{
		ID3D11Buffer* VertexBuffer;
		UINT Stride;
		MeshAssetInstance->GetVertexInformation(VertexBuffer, Stride);

		ID3D11Buffer* VertexBuffers[] = { VertexBuffer };
		UINT Strides[] = { Stride };
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
