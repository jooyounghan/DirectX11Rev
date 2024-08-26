#include "AObject.h"
#include "GraphicsPipeline.h"
#include <format>

using namespace std;

AObject::AObject(GraphicsPipeline* GraphicsPipelineInstance)
	: ObjectID(string{ format("{}", (uint64_t)this) }), 
	RelativePositionID(string{ format("{}", (uint64_t)&RelativePosition) }),
	RelativeAngleID(string{ format("{}", (uint64_t)&RelativeAngle) }),
	RelativeScaleID(string{ format("{}", (uint64_t)&RelativeScale) }),
	DeviceContextCached(GraphicsPipelineInstance->GetDeviceContext()), 
	TransformationBuffer(GraphicsPipelineInstance->GetDevice())
{
	AutoZeroMemory(RelativePosition);
	AutoZeroMemory(RelativeScale);
	AutoZeroMemory(RelativeAngle);

	RelativeScale.x = 1.f;
	RelativeScale.y = 1.f;
	RelativeScale.z = 1.f;
}

void AObject::UpdateObject(const float& DeltaTimeIn)
{
	TransformationMatrix TempTransformation;

	TempTransformation.TransfomationMat = GetTransformation();
	TempTransformation.InvTransfomationMat = XMMatrixInverse(nullptr, TempTransformation.TransfomationMat);
	TempTransformation.TransfomationMat = XMMatrixTranspose(TempTransformation.TransfomationMat);

	TransformationBuffer.Upload(DeviceContextCached, TempTransformation);
}

void AObject::OnSerialize(FILE* FileIn)
{
	// Object Name
	size_t PlaceableNameCount = ObjectName.size();
	fwrite(&PlaceableNameCount, sizeof(size_t), 1, FileIn);
	fwrite(ObjectName.c_str(), sizeof(char), PlaceableNameCount, FileIn);

	// Transformation
	fwrite(&RelativePosition, sizeof(SPosition4D), 1, FileIn);
	fwrite(&RelativeAngle, sizeof(SAngle), 1, FileIn);
	fwrite(&RelativeScale, sizeof(SVector3D), 1, FileIn);
}

void AObject::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	// Object Name
	size_t PlaceableNameCount;
	fread(&PlaceableNameCount, sizeof(size_t), 1, FileIn);
	ObjectName.resize(PlaceableNameCount);
	fread(ObjectName.data(), sizeof(char), PlaceableNameCount, FileIn);

	// Transformation
	fread(&RelativePosition, sizeof(SPosition4D), 1, FileIn);
	fread(&RelativeAngle, sizeof(SAngle), 1, FileIn);
	fread(&RelativeScale, sizeof(SVector3D), 1, FileIn);
}