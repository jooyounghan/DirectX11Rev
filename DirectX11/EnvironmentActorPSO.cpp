#include "EnvironmentActorPSO.h"
#include "Camera.h"
#include "EnvironmentActor.h"
#include "EXRTextureAsset.h"
#include "BaseMeshAsset.h"

using namespace std;
using namespace Microsoft::WRL;

EnvironmentActorPSO::EnvironmentActorPSO(
	ID3D11DeviceContext* DeviceContextIn, 
	const ComPtr<ID3D11InputLayout>& InputLayoutIn, 
	const ComPtr<ID3D11VertexShader>& VertexShaderIn, 
	const UINT& NumVSConstBuffersIn, 
	const UINT& NumVSSRVsIn, 
	const ComPtr<ID3D11PixelShader>& PixelShaderIn, 
	const UINT& NumPSConstBuffersIn, 
	const UINT& NumPSSRVsIn,
	const D3D11_PRIMITIVE_TOPOLOGY& PrimitiveTopologyIn, 
	const UINT& NumRenderTargetsIn, 
	const DXGI_FORMAT* const RTVFormatsIn, 
	const DXGI_FORMAT& DSVFormatIn, 
	const DXGI_SAMPLE_DESC& SampleDescIn, 
	const ComPtr<ID3D11RasterizerState> RasterizerStateIn, 
	const ComPtr<ID3D11DepthStencilState> DepthStencilStateIn, 
	const UINT& StencilRefIn, 
	const ComPtr<ID3D11BlendState> BlendStateIn, 
	const vector<ComPtr<ID3D11SamplerState>>& SamplerStatesIn
)
	: APSOObject(DeviceContextIn, InputLayoutIn, VertexShaderIn, NumVSConstBuffersIn,
		NumVSSRVsIn, PixelShaderIn, NumPSConstBuffersIn, NumPSSRVsIn, PrimitiveTopologyIn,
		NumRenderTargetsIn, RTVFormatsIn, DSVFormatIn, SampleDescIn, RasterizerStateIn,
		DepthStencilStateIn, StencilRefIn, BlendStateIn, SamplerStatesIn
	) {}

void EnvironmentActorPSO::PresetRendering(Camera* CameraIn, MapAsset* MapIn)
{
	ID3D11RenderTargetView* RTVs[]{ CameraIn->GetHDRSceneRTV() };

	SetPipelineStateObject(1, RTVs, &CameraIn->GetViewport(), CameraIn->GetSceneDSV());
	ID3D11Buffer* ViewProjBuffer[] = { CameraIn->ViewProjBuffer.GetBuffer() };
	SetVSConstantBuffers(0, 1, ViewProjBuffer);
}

void EnvironmentActorPSO::Render()
{
	for (EnvironmentActor* EnvironmentActorObject : EnvironmentActors)
	{
		BaseMeshAsset* EnvironmentMeshAsset = EnvironmentActorObject->GetEnvironmentMeshAsset();

		if (EnvironmentMeshAsset != nullptr)
		{
			const std::vector<ID3D11Buffer*> VertexBuffers = EnvironmentMeshAsset->GetVertexBuffers();
			const std::vector<UINT> Strides = EnvironmentMeshAsset->GetStrides();
			const std::vector<UINT> Offsets = EnvironmentMeshAsset->GetOffsets();

			DeviceContextCached->IASetVertexBuffers(
				0, static_cast<UINT>(VertexBuffers.size()),
				VertexBuffers.data(),
				Strides.data(),
				Offsets.data()
			);
			DeviceContextCached->IASetIndexBuffer(EnvironmentMeshAsset->GetIndexBuffer(), EnvironmentMeshAsset->GetIndexFormat(), 0);

			ID3D11Buffer* VSConstBuffers[] = { EnvironmentActorObject->TransformationBuffer.GetBuffer() };

			SetVSConstantBuffers(1, 1, VSConstBuffers);

			EXRTextureAsset* BackgroundEXRTextureAsset = EnvironmentActorObject->GetEnvironmentBackgroundEXRTextureAsset();

			if (BackgroundEXRTextureAsset != nullptr)
			{
				ID3D11ShaderResourceView* SRVs[] = { BackgroundEXRTextureAsset->GetSRV() };
				SetPSShaderResourceViews(0, 1, SRVs);
			}

#ifdef _DEBUG
			CheckPipelineValidation();
#endif // DEBUG

			DeviceContextCached->DrawIndexed(static_cast<UINT>(EnvironmentMeshAsset->GetIndexCount()), 0, 0);

			ResetVSConstantBuffers(1, 1);
			ResetPSShaderResourceViews(0, 1);
		}

	}
}

void EnvironmentActorPSO::ResetRendering()
{
	ResetVSConstantBuffers(0, 1);
}
