#include "MeshObject.h"
#include "GraphicsPipeline.h"
#include "AMeshAsset.h"
#include "PSOObject.h"
#include "IGuiLowLevelVisitor.h"

using namespace std;

MeshObject::MeshObject(
	GraphicsPipeline* GraphicsPipelineInstances, 
	AMeshAsset* MeshAssetInstanceIn
)
	: RelativePlaceableObject(
		GraphicsPipelineInstances->GetDevice(),
		GraphicsPipelineInstances->GetDeviceContext()
	), MeshAssetInstance(MeshAssetInstanceIn)
{
	static size_t MeshObjectCount = 0;
	MeshObjectCount++;
	ObjectName = "Mesh Object " + to_string(MeshObjectCount);

}

MeshObject::~MeshObject()
{
}

void MeshObject::UpdateObject(const float& DeltaTimeIn)
{
	RelativePlaceableObject::UpdateObject(DeltaTimeIn);
}

void MeshObject::Render(PSOObject* PSOObjectIn)
{
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

		ID3D11Buffer* VSConstBuffers[] = { TransformationBuffer.GetBuffer() };

		PSOObjectIn->SetVSConstantBuffers(1, 1, VSConstBuffers);

#ifdef _DEBUG
		PSOObjectIn->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextCached->DrawIndexed(static_cast<UINT>(MeshAssetInstance->GetIndexCount()), 0, 0);

		PSOObjectIn->ResetVSConstantBuffers(1, 1);
	}
}

void MeshObject::AcceptGui(IGuiLowLevelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}
