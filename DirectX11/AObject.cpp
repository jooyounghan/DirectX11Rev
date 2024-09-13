#include "AObject.h"
#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include <format>

using namespace std;

AObject::AObject()
	: ObjectID(string{ format("{}", (uint64_t)this) }), 
	RelativePositionID(string{ format("{}", (uint64_t)&RelativePosition) }),
	RelativeAngleID(string{ format("{}", (uint64_t)&RelativeAngle) }),
	RelativeScaleID(string{ format("{}", (uint64_t)&RelativeScale) }),
	DeviceContextCached(App::GGraphicPipeline->GetDeviceContext()),
	TransformationBuffer()
{
	AutoZeroMemory(RelativePosition);
	AutoZeroMemory(RelativeScale);
	AutoZeroMemory(RelativeAngle);

	RelativeScale.x = 1.f;
	RelativeScale.y = 1.f;
	RelativeScale.z = 1.f;
}

void AObject::Update(const float& DeltaTimeIn)
{
	TransformationMatrix TempTransformation;

	TempTransformation.TransfomationMat = GetTransformation();
	TempTransformation.InvTransfomationMat = XMMatrixInverse(nullptr, TempTransformation.TransfomationMat);
	TempTransformation.TransfomationMat = XMMatrixTranspose(TempTransformation.TransfomationMat);

	TransformationBuffer.Upload(TempTransformation);
}

void AObject::OnSerializeFromMap(FILE* FileIn)
{
	// Object Name
	SerializeString(ObjectName, FileIn);

	// Transformation
	fwrite(&RelativePosition, sizeof(XMFLOAT3), 1, FileIn);
	fwrite(&RelativeAngle, sizeof(XMFLOAT3), 1, FileIn);
	fwrite(&RelativeScale, sizeof(XMFLOAT3), 1, FileIn);
}

void AObject::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	// Object Name
	DeserializeString(ObjectName, FileIn);

	// Transformation
	fread(&RelativePosition, sizeof(XMFLOAT3), 1, FileIn);
	fread(&RelativeAngle, sizeof(XMFLOAT3), 1, FileIn);
	fread(&RelativeScale, sizeof(XMFLOAT3), 1, FileIn);
}

void AObject::SerializeString(const std::string& String, FILE* FileIn)
{
	// Object Name
	size_t NameCount = String.size();
	fwrite(&NameCount, sizeof(size_t), 1, FileIn);
	fwrite(String.c_str(), sizeof(char), NameCount, FileIn);
}

void AObject::DeserializeString(std::string& String, FILE* FileIn)
{
	// Object Name
	size_t NameCount;
	fread(&NameCount, sizeof(size_t), 1, FileIn);
	String.resize(NameCount);
	fread(String.data(), sizeof(char), NameCount, FileIn);
}
