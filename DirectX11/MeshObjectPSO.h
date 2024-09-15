#pragma once
#include "APSOObject.h"
#include "Camera.h"
#include "AMeshAsset.h"
#include <list>

template<typename MeshObject>
class MeshObjectPSO : public APSOObject
{
public:
	MeshObjectPSO(
		ID3D11DeviceContext* DeviceContextIn,
		const Microsoft::WRL::ComPtr<ID3D11InputLayout>& InputLayoutIn,
		const Microsoft::WRL::ComPtr<ID3D11VertexShader>& VertexShaderIn,
		const UINT& NumVSConstBuffersIn,
		const UINT& NumVSSRVsIn,
		const Microsoft::WRL::ComPtr<ID3D11PixelShader>& PixelShaderIn,
		const UINT& NumPSConstBuffersIn,
		const UINT& NumPSSRVsIn,
		const D3D11_PRIMITIVE_TOPOLOGY& PrimitiveTopologyIn,
		const UINT& NumRenderTargetsIn,
		const DXGI_FORMAT* const										RTVFormatsIn,
		const DXGI_FORMAT& DSVFormatIn,
		const DXGI_SAMPLE_DESC& SampleDescIn,
		const Microsoft::WRL::ComPtr<ID3D11RasterizerState>				RasterizerStateIn,
		const Microsoft::WRL::ComPtr<ID3D11DepthStencilState>			DepthStencilStateIn,
		const UINT& StencilRefIn,
		const Microsoft::WRL::ComPtr<ID3D11BlendState>					BlendStateIn,
		const std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>>& SamplerStatesIn
	)
	: APSOObject(DeviceContextIn, InputLayoutIn, VertexShaderIn, NumVSConstBuffersIn,
		NumVSSRVsIn, PixelShaderIn, NumPSConstBuffersIn, NumPSSRVsIn, PrimitiveTopologyIn,
		NumRenderTargetsIn, RTVFormatsIn, DSVFormatIn, SampleDescIn, RasterizerStateIn,
		DepthStencilStateIn, StencilRefIn, BlendStateIn, SamplerStatesIn
	) {}
	virtual ~MeshObjectPSO() {};

protected:
	ManageForRendering(MeshObject);

public:
	virtual void PresetRendering(Camera* CameraIn, MapAsset* MapIn) override;
	virtual void Render() override;
	virtual void ResetRendering() override;
};


template<typename MeshObject>
void MeshObjectPSO<MeshObject>::PresetRendering(Camera* CameraIn, MapAsset* MapIn)
{
	ID3D11RenderTargetView* RTVs[]{ CameraIn->GetSceneRTV() };

	SetPipelineStateObject(1, RTVs, &CameraIn->GetViewport(), CameraIn->GetSceneDSV());
	ID3D11Buffer* ViewProjBuffer[] = { CameraIn->ViewProjBuffer.GetBuffer() };
	SetVSConstantBuffers(0, 1, ViewProjBuffer);
}

template<typename MeshObject>
void MeshObjectPSO<MeshObject>::Render()
{
	for (MeshObject* RenderMeshObject : MeshObjects)
	{
		AMeshAsset* MeshAssetInstance = RenderMeshObject->GetMeshAssetInstance();
		if (RenderMeshObject->GetIsRenderable() && MeshAssetInstance)
		{
			const std::vector<ID3D11Buffer*> VertexBuffers = MeshAssetInstance->GetVertexBuffers();
			const std::vector<UINT> Strides = MeshAssetInstance->GetStrides();
			const std::vector<UINT> Offsets = MeshAssetInstance->GetOffsets();

			DeviceContextCached->IASetVertexBuffers(0, static_cast<UINT>(VertexBuffers.size()),
				VertexBuffers.data(),
				Strides.data(),
				Offsets.data()
			);
			DeviceContextCached->IASetIndexBuffer(MeshAssetInstance->GetIndexBuffer(), MeshAssetInstance->GetIndexFormat(), 0);

			ID3D11Buffer* VSConstBuffers[] = { RenderMeshObject->TransformationBuffer.GetBuffer() };

			SetVSConstantBuffers(1, 1, VSConstBuffers);

#ifdef _DEBUG
			CheckPipelineValidation();
#endif // DEBUG

			DeviceContextCached->DrawIndexed(static_cast<UINT>(MeshAssetInstance->GetIndexCount()), 0, 0);

			ResetVSConstantBuffers(1, 1);
		}
	}
}

template<typename MeshObject>
void MeshObjectPSO<MeshObject>::ResetRendering()
{
	ResetVSConstantBuffers(0, 1);
}
