#include "ABoundingObject.h"
#include "GlobalVariable.h"
#include "Debugable.h"

#include "PSOManager.h"
#include "PSOObject.h"

#include "MapAsset.h"
#include "ACamera.h"

#include "UploadableBufferManager.h"

#include <vector>

using namespace std;
using namespace DirectX;

ABoundingObject::ABoundingObject(const std::string& AttachableKindIn)
	: AAttachableObject(AttachableKindIn)
{
	DebuggingColorBuffer = App::GUploadableBufferManager->CreateUploadableBuffer<UploadBuffer<XMVECTOR>>();
	//BoundingObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Forward_Bounding_Wireframe);
	BoundingObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Deffered_Bounding_Wireframe);
}

ABoundingObject::~ABoundingObject()
{
}

void ABoundingObject::SetCollisionColor()
{
	if (DebugObject != nullptr)
	{
		if (IsCollided)
		{
			DebuggingColorBuffer->SetStagedData(XMVectorSet(0.f, 1.f, 0.f, 1.f));
		}
		else
		{
			DebuggingColorBuffer->SetStagedData(XMVectorSet(1.f, 0.f, 0.f, 1.f));
		}
	}
}

void ABoundingObject::Render(MapAsset* MapAssetIn)
{
	AAttachableObject::Render(MapAssetIn);

	ACamera* CurrentCamera = MapAssetIn->GetCurrentCamera();
	if (CurrentCamera != nullptr && DebugObject != nullptr)
	{
		ID3D11Buffer* VertexBuffers[] = { DebugObject->GetVertexBuffer() };
		UINT Strides[] = { DebugObject->GetVertexTypeSize() };
		UINT Offsets[] = { 0 };
		ID3D11Buffer* VSConstBuffers[] = { CurrentCamera->GetViewProjBuffer()->GetBuffer(), TransformationBuffer->GetBuffer()};
		ID3D11Buffer* PSConstBuffers[] = { DebuggingColorBuffer->GetBuffer(), PickingIDBufferCached };

#pragma region BoundingObjectPSOCached

		//ID3D11RenderTargetView* SDRRTVs[]{ CurrentCamera->GetSDRSceneRTV(), CurrentCamera->GetIdSelectRTV()};
		//BoundingObjectPSOCached->SetPipelineStateObject(
		//	SDRRTVs.size(), SDRRTVs.data(), &CurrentCamera->GetViewport(),
		//	CurrentCamera->GetSceneDSV()
		//);

		vector<ID3D11RenderTargetView*> SDRRTVs{ 
			CurrentCamera->GetGBufferRTV(Position_GBuffer), CurrentCamera->GetGBufferRTV(BaseColor_GBuffer),
			CurrentCamera->GetGBufferRTV(Normal_GBuffer), CurrentCamera->GetGBufferRTV(AO_Metallic_Roughness_GBuffer),
			CurrentCamera->GetGBufferRTV(Emissive_GBuffer), CurrentCamera->GetIdSelectRTV() 
		};

		BoundingObjectPSOCached->SetPipelineStateObject(
			SDRRTVs.size(), SDRRTVs.data(), &CurrentCamera->GetViewport(),
			CurrentCamera->GetGBufferDSV()
		);

		DeviceContextCached->IASetVertexBuffers(0, 1, VertexBuffers, Strides, Offsets);
		DeviceContextCached->IASetIndexBuffer(DebugObject->GetIndexBuffer(), DebugObject->GetIndexFormat(), 0);

		BoundingObjectPSOCached->SetVSConstantBuffers(0, 2, VSConstBuffers);
		BoundingObjectPSOCached->SetPSConstantBuffers(0, 2, PSConstBuffers);

#ifdef _DEBUG
		BoundingObjectPSOCached->CheckPipelineValidation();
#endif
		DeviceContextCached->DrawIndexed(static_cast<UINT>(DebugObject->GetIndexCount()), 0, 0);

		BoundingObjectPSOCached->ResetVSConstantBuffers(0, 2);
		BoundingObjectPSOCached->ResetPSConstantBuffers(0, 2);
#pragma endregion
	}
}
