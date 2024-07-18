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
		// TODO : MeshAsset에서는 Vertex를 얻을 수 현재 없다 ID3D11Buffer* 반환하는 순수가상함수 ㄱㄱ
		// TODO : 매번 바뀌는 버퍼의 경우 Set된 플래그를 풀어줄 필요가 있음!!!! PSOObject에 플래그 해제 함수 세팅
		DeviceContextCached->IASetVertexBuffers(0, 1, MeshAssetInstance->)
		CommandListIn->IASetVertexBuffers(0, 1, &(MeshAssetInstance->GetVertexBufferView()));
		CommandListIn->IASetIndexBuffer(&(MeshAssetInstance->GetIndexBufferView()));

		CommandListIn->SetGraphicsRootConstantBufferView(0, TransformationBuffer.GetBufferAddress());
		CommandListIn->SetGraphicsRootConstantBufferView(1, CameraIn->ViewProjBuffer.GetBufferAddress());

		CommandListIn->DrawIndexedInstanced(static_cast<UINT>(MeshAssetInstance->GetIndexCount()), 1, 0, 0, 0);
	}
}
