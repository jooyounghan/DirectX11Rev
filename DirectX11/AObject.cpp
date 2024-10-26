#include "AObject.h"
#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "AAssetFile.h"
#include "UploadBuffer.h"
#include "UploadableBufferManager.h"

#include <format>

using namespace std;
using namespace DirectX;

AObject::AObject(MapAsset* MapAssetInstance)
	: AMovable(), MapAssetCached(MapAssetInstance),
	DeviceContextCached(App::GGraphicPipeline->GetDeviceContext()),
	ObjectID(string{ format("{}", (uint64_t)this) })
{
	TransformationBuffer = App::GUploadableBufferManager->CreateUploadableBuffer<UploadBuffer<TransformationMatrix>>();
}

void AObject::Update(const float& DeltaTimeIn)
{
	TransformationMatrix TempTransformation;

	TempTransformation.TransfomationMat = GetTransformation();
	TempTransformation.InvTransfomationMat = XMMatrixInverse(nullptr, TempTransformation.TransfomationMat);
	TempTransformation.TransfomationMat = XMMatrixTranspose(TempTransformation.TransfomationMat);

	TransformationBuffer->SetStagedData(TempTransformation);
}

void AObject::OnSerializeFromMap(FILE* FileIn)
{
	// Object Name
	AAssetFile::SerializeString(ObjectName, FileIn);

	// Transformation
	fwrite(&RelativePosition, sizeof(XMFLOAT3), 1, FileIn);
	fwrite(&RelativeAngle, sizeof(XMFLOAT3), 1, FileIn);
	fwrite(&RelativeScale, sizeof(XMFLOAT3), 1, FileIn);
}

void AObject::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	// Object Name
	AAssetFile::DeserializeString(ObjectName, FileIn);

	// Transformation
	fread(&RelativePosition, sizeof(XMFLOAT3), 1, FileIn);
	fread(&RelativeAngle, sizeof(XMFLOAT3), 1, FileIn);
	fread(&RelativeScale, sizeof(XMFLOAT3), 1, FileIn);
}

