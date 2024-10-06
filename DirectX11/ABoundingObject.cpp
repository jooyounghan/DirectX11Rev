#include "ABoundingObject.h"
#include "GlobalVariable.h"
#include "Debugable.h"

#include "PSOManager.h"
#include "PSOObject.h"

#include "MapAsset.h"
#include "Camera.h"

using namespace DirectX;

ABoundingObject::ABoundingObject(MapAsset* MapAssetInstance)
	: AAttachableObject(MapAssetInstance), DebuggingColorBuffer()
{
	BoundingObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::BoundingObject_Wireframe);
	PickingIDWireframePSOCached = App::GPSOManager->GetPSOObject(EPSOType::BoundingObject_ID_Wireframe);
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
			DebuggingColorBuffer.Upload(XMVectorSet(0.f, 1.f, 0.f, 1.f));
		}
		else
		{
			DebuggingColorBuffer.Upload(XMVectorSet(1.f, 0.f, 0.f, 1.f));
		}
	}
}

void ABoundingObject::Render()
{
	AAttachableObject::Render();

	Camera* CurrentCamera = MapAssetCached->GetCurrentCamera();
	if (CurrentCamera != nullptr && DebugObject != nullptr)
	{
		ID3D11Buffer* VertexBuffers[] = { DebugObject->GetVertexBuffer() };
		UINT Strides[] = { DebugObject->GetVertexTypeSize() };
		UINT Offsets[] = { 0 };
		ID3D11Buffer* VSConstBuffers[] = { CurrentCamera->ViewProjBuffer.GetBuffer(), TransformationBuffer.GetBuffer() };
		ID3D11Buffer* PSConstBuffers[] = { GetDebuggingColorBuffer().GetBuffer() };

#pragma region BoundingObjectPSOCached
		ID3D11RenderTargetView* SDRRTVs[]{ CurrentCamera->GetSDRSceneRTV() };

		BoundingObjectPSOCached->SetPipelineStateObject(
			1, SDRRTVs, &CurrentCamera->GetViewport(), 
			CurrentCamera->GetSceneDSV()
		);

		DeviceContextCached->IASetVertexBuffers(0, 1, VertexBuffers, Strides, Offsets);
		DeviceContextCached->IASetIndexBuffer(DebugObject->GetIndexBuffer(), DebugObject->GetIndexFormat(), 0);

		BoundingObjectPSOCached->SetVSConstantBuffers(0, 2, VSConstBuffers);
		BoundingObjectPSOCached->SetPSConstantBuffers(0, 1, PSConstBuffers);

#ifdef _DEBUG
		BoundingObjectPSOCached->CheckPipelineValidation();
#endif
		DeviceContextCached->DrawIndexed(static_cast<UINT>(DebugObject->GetIndexCount()), 0, 0);

		BoundingObjectPSOCached->ResetVSConstantBuffers(0, 2);
		BoundingObjectPSOCached->ResetPSConstantBuffers(0, 1);
#pragma endregion

#pragma region PickingIDWireframePSOCached
		ID3D11RenderTargetView* IDRTVs[]{ CurrentCamera->GetIdSelectRTV() };

		PickingIDWireframePSOCached->SetPipelineStateObject(1, IDRTVs, &CurrentCamera->GetViewport(), CurrentCamera->GetIdSelectDSV());

		DeviceContextCached->IASetVertexBuffers(0, 1, VertexBuffers, Strides, Offsets);
		DeviceContextCached->IASetIndexBuffer(DebugObject->GetIndexBuffer(), DebugObject->GetIndexFormat(), 0);

		PickingIDWireframePSOCached->SetVSConstantBuffers(0, 2, VSConstBuffers);
		PickingIDWireframePSOCached->SetPSConstantBuffers(0, 1, PSConstBuffers);

#ifdef _DEBUG
		PickingIDWireframePSOCached->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextCached->DrawIndexed(static_cast<UINT>(DebugObject->GetIndexCount()), 0, 0);

		PickingIDWireframePSOCached->ResetVSConstantBuffers(0, 2);
		PickingIDWireframePSOCached->ResetPSConstantBuffers(0, 1);

#pragma endregion
	}


}
