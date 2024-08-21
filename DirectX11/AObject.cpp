#include "AObject.h"
#include <format>

using namespace std;

AObject::AObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: ObjectID(string{ format("{}", (uint64_t)this) }), DeviceContextCached(DeviceContextIn), TransformationBuffer(DeviceIn)
{
	AutoZeroMemory(Position);
	AutoZeroMemory(Scale);
	AutoZeroMemory(Angle);

	Scale.x = 1.f;
	Scale.y = 1.f;
	Scale.z = 1.f;
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
	fwrite(&Position, sizeof(SPosition4D), 1, FileIn);
	fwrite(&Angle, sizeof(SAngle), 1, FileIn);
	fwrite(&Scale, sizeof(SVector3D), 1, FileIn);
}

void AObject::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	// Object Name
	size_t PlaceableNameCount;
	fread(&PlaceableNameCount, sizeof(size_t), 1, FileIn);
	ObjectName.resize(PlaceableNameCount);
	fread(ObjectName.data(), sizeof(char), PlaceableNameCount, FileIn);

	// Transformation
	fread(&Position, sizeof(SPosition4D), 1, FileIn);
	fread(&Angle, sizeof(SAngle), 1, FileIn);
	fread(&Scale, sizeof(SVector3D), 1, FileIn);
}
