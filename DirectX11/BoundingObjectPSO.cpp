#include "BoundingObjectPSO.h"

#include "Camera.h"

#include "Debugable.h"
#include "ABoundingObject.h"

using namespace std;
using namespace Microsoft::WRL;

BoundingObjectPSO::BoundingObjectPSO(
	ID3D11DeviceContext*						DeviceContextIn,
	const ComPtr<ID3D11InputLayout>&			InputLayoutIn,
	const ComPtr<ID3D11VertexShader>&			VertexShaderIn,
	const UINT&									NumVSConstBuffersIn,
	const UINT&									NumVSSRVsIn,
	const ComPtr<ID3D11PixelShader>&			PixelShaderIn,
	const UINT&									NumPSConstBuffersIn,
	const UINT&									NumPSSRVsIn,
	const D3D11_PRIMITIVE_TOPOLOGY&				PrimitiveTopologyIn,
	const UINT&									NumRenderTargetsIn,
	const DXGI_FORMAT*							RTVFormatsIn,
	const DXGI_FORMAT&							DSVFormatIn,
	const DXGI_SAMPLE_DESC&						SampleDescIn,
	const ComPtr<ID3D11RasterizerState>			RasterizerStateIn,
	const ComPtr<ID3D11DepthStencilState>		DepthStencilStateIn,
	const UINT&									StencilRefIn,
	const ComPtr<ID3D11BlendState>				BlendStateIn,
	const vector<ComPtr<ID3D11SamplerState>>&	SamplerStatesIn
)
	: APSOObject(DeviceContextIn, InputLayoutIn, VertexShaderIn, NumVSConstBuffersIn,
		NumVSSRVsIn, PixelShaderIn, NumPSConstBuffersIn, NumPSSRVsIn, PrimitiveTopologyIn,
		NumRenderTargetsIn, RTVFormatsIn, DSVFormatIn, SampleDescIn, RasterizerStateIn,
		DepthStencilStateIn, StencilRefIn, BlendStateIn, SamplerStatesIn
	)
{
}

BoundingObjectPSO::~BoundingObjectPSO()
{
}

void BoundingObjectPSO::PresetRendering(Camera* CameraIn, MapAsset* MapIn)
{
	ID3D11RenderTargetView* RTVs[]{ CameraIn->GetSDRSceneRTV() };

	SetPipelineStateObject(1, RTVs, &CameraIn->GetViewport(), CameraIn->GetSceneDSV());
	ID3D11Buffer* ViewProjBuffer[] = { CameraIn->ViewProjBuffer.GetBuffer() };
	SetVSConstantBuffers(0, 1, ViewProjBuffer);
}

void BoundingObjectPSO::Render()
{
	for (ABoundingObject* RenderBoundingObject : ABoundingObjects)
	{
		Debugable* DebugObject = RenderBoundingObject->GetDebugObject();

		if (RenderBoundingObject->GetIsRenderable() && DebugObject)
		{
			ID3D11Buffer* VertexBuffers[] = { DebugObject->GetVertexBuffer() };
			UINT Strides[] = { DebugObject->GetVertexTypeSize() };
			UINT Offsets[] = { 0 };

			DeviceContextCached->IASetVertexBuffers(0, 1, VertexBuffers, Strides, Offsets);
			DeviceContextCached->IASetIndexBuffer(DebugObject->GetIndexBuffer(), DebugObject->GetIndexFormat(), 0);

			ID3D11Buffer* VSConstBuffers[] = { RenderBoundingObject->TransformationBuffer.GetBuffer() };
			ID3D11Buffer* PSConstBuffers[] = { RenderBoundingObject->GetDebuggingColorBuffer().GetBuffer() };

			SetVSConstantBuffers(1, 1, VSConstBuffers);
			SetPSConstantBuffers(0, 1, PSConstBuffers);
#ifdef _DEBUG
			CheckPipelineValidation();
#endif

			DeviceContextCached->DrawIndexed(static_cast<UINT>(DebugObject->GetIndexCount()), 0, 0);

			ResetVSConstantBuffers(1, 1);
			ResetPSConstantBuffers(0, 1);
		}
	}
}

void BoundingObjectPSO::ResetRendering()
{
	ResetVSConstantBuffers(0, 1);
}
