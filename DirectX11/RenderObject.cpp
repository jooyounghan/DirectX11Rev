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
		// TODO : MeshAsset������ Vertex�� ���� �� ���� ���� ID3D11Buffer* ��ȯ�ϴ� ���������Լ� ����
		// TODO : �Ź� �ٲ�� ������ ��� Set�� �÷��׸� Ǯ���� �ʿ䰡 ����!!!! PSOObject�� �÷��� ���� �Լ� ����
		DeviceContextCached->IASetVertexBuffers(0, 1, MeshAssetInstance->)
		CommandListIn->IASetVertexBuffers(0, 1, &(MeshAssetInstance->GetVertexBufferView()));
		CommandListIn->IASetIndexBuffer(&(MeshAssetInstance->GetIndexBufferView()));

		CommandListIn->SetGraphicsRootConstantBufferView(0, TransformationBuffer.GetBufferAddress());
		CommandListIn->SetGraphicsRootConstantBufferView(1, CameraIn->ViewProjBuffer.GetBufferAddress());

		CommandListIn->DrawIndexedInstanced(static_cast<UINT>(MeshAssetInstance->GetIndexCount()), 1, 0, 0, 0);
	}
}
