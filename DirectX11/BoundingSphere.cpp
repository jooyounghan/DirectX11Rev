#include "BoundingSphere.h"
#include "GraphicsPipeline.h"
#include "PSOObject.h"
#include "DefineUtility.h"


BoundingSphere::BoundingSphere(
	GraphicsPipeline* GraphicsPipelineInstances,
    const float& RadiusIn, Debugable* DebugObjectIn
)
    : AttachableObject(
		GraphicsPipelineInstances->GetDevice(), 
		GraphicsPipelineInstances->GetDeviceContext()
	), DebugObject(DebugObjectIn)
{
	AutoZeroMemory(ParentPosition);
    Scale = RadiusIn;
}

BoundingSphere::~BoundingSphere() 
{

}

bool BoundingSphere::Intersect(const Ray& RayIn, float& DistanceOut)
{
    const float& Radius = Scale;
	SPosition3D Center = SPosition3D{
		ParentPosition.x + Position.x,
		ParentPosition.y + Position.y,
		ParentPosition.z + Position.z

	};
	float b = InnerProduct(RayIn.Direction, RayIn.Origin - Center);
	float c = InnerProduct(RayIn.Origin - Center, RayIn.Origin - Center) - Radius * Radius;

	float determinant = b * b - c;
	if (determinant < 0) 
	{ 
		return false; 
	}
	else
	{
		if (b > 0)
		{
			return false;
		}
		else
		{
			if (determinant == 0)
			{
				DistanceOut = -b;
			}
			else
			{
				float t1 = -b - sqrtf(b * b - c);
				float t2 = -b + sqrtf(b * b - c);
				DistanceOut = t1 < t2 ? t1 : t2;
			}
			return true;
		}
	}
}

void BoundingSphere::UpdateObject(const float& DeltaTimeIn, IObject* ParentObject)
{
	memcpy(&ParentPosition, &(ParentObject->Position), sizeof(ParentPosition));
	AttachableObject::UpdateObject(DeltaTimeIn, ParentObject);
}

void BoundingSphere::Render(PSOObject* PSOObjectIn)
{
	if (DebugObject)
	{

		ID3D11Buffer* VertexBuffers[] = { DebugObject->GetVertexBuffer()};
		UINT Strides[] = { DebugObject->GetVertexTypeSize() };
		UINT Offsets[] = { 0 };

		DeviceContextCached->IASetVertexBuffers(0, 1, VertexBuffers, Strides, Offsets);
		DeviceContextCached->IASetIndexBuffer(DebugObject->GetIndexBuffer(), DebugObject->GetIndexFormat(), 0);

		ID3D11Buffer* VSConstBuffers[] = { TransformationBuffer.GetBuffer() };
		PSOObjectIn->SetVSConstantBuffers(1, 1, VSConstBuffers);

#ifdef _DEBUG
		PSOObjectIn->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextCached->DrawIndexed(static_cast<UINT>(DebugObject->GetIndexCount()), 0, 0);

		PSOObjectIn->ResetVSConstantBuffers(1, 0);
	}
}                                    