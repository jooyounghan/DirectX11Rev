#include "ComponentInitializer.h"

#include "Scene.h"
#include "LightManager.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"
#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"

#include "SpotLightComponent.h"
#include "PointLightComponent.h"

#include "SkeletalMeshAsset.h"
#include "AnimationPlayer.h"

#include "ConstantBuffer.h"
#include "DynamicBuffer.h"

#include "Texture2DInstance.h"
#include "RTVOption.h"
#include "SRVOption.h"
#include "UAVOption.h"
#include "DSVOption.h"

using namespace std;
using namespace Microsoft::WRL;

constexpr uint8_t initializeOption = ~((uint8_t)0);

ComponentInitializer::ComponentInitializer(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: m_deviceCached(device), m_deviceContextCached(deviceContext)
{
}

void ComponentInitializer::Visit(Scene* scene)
{
	LightManager& lightManager = scene->GetLightManager();
	
	Texture2DInstance<SRVOption>& spotLightDepthTestViews = lightManager.GetSpotLightDepthTestViews();
	array<ComPtr<ID3D11DepthStencilView>, MaxSpotLightCount>& spotLightDSVs = lightManager.GetSpotLightDSVs();

	spotLightDepthTestViews.InitializeByOption(m_deviceCached, m_deviceContextCached);
	
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsvDesc.Texture2DArray.MipSlice = 0;
	dsvDesc.Texture2DArray.ArraySize = 1;

	constexpr UINT maxSpotLightCount = static_cast<UINT>(MaxSpotLightCount);
	for (UINT idx = 0; idx < maxSpotLightCount; ++idx)
	{
		dsvDesc.Texture2DArray.FirstArraySlice = idx;
		m_deviceCached->CreateDepthStencilView(
			spotLightDepthTestViews.GetTexture2D(),
			&dsvDesc,
			spotLightDSVs[idx].GetAddressOf()
		);
	}

	Texture2DInstance<SRVOption>& pointLightDepthTestViews = lightManager.GetPointLightDepthTestViews();
	array<array<ComPtr<ID3D11DepthStencilView>, 6>, MaxSpotLightCount>& pointLightCubeDSVs = lightManager.GetPointLightCubeDSVs();
	pointLightDepthTestViews.InitializeByOption(m_deviceCached, m_deviceContextCached);

	constexpr UINT maxPointLightCount = static_cast<UINT>(MaxPointLightCount);

	for (UINT lightIdx = 0; lightIdx < maxPointLightCount; ++lightIdx)
	{
		for (UINT faceIdx = 0; faceIdx < 6; ++faceIdx)
		{
			dsvDesc.Texture2DArray.FirstArraySlice = lightIdx * 6 + faceIdx;
			m_deviceCached->CreateDepthStencilView(
				pointLightDepthTestViews.GetTexture2D(),
				&dsvDesc,
				pointLightCubeDSVs[lightIdx][faceIdx].GetAddressOf()
			);
		}
	}

	lightManager.GetLightManagerEntityBuffer().InitializeBuffer(m_deviceCached);

	lightManager.GetSpotLightEntitiesBuffer().InitializeBuffer(m_deviceCached);
	lightManager.GetSpotLightViewEntitiesBuffer().InitializeBuffer(m_deviceCached);

	lightManager.GetPointLightEntitiesBuffer().InitializeBuffer(m_deviceCached);
	lightManager.GetPointLightPositionsBuffer().InitializeBuffer(m_deviceCached);
}

void ComponentInitializer::Visit(StaticMeshComponent* staticModelComponent)
{
	InitBaseComponent(staticModelComponent);
}

void ComponentInitializer::Visit(SkeletalMeshComponent* skeletalModelComponent)
{
	InitBaseComponent(skeletalModelComponent);

	const SkeletalMeshAsset* skeletalMetalAsset = skeletalModelComponent->GetSkeletalMetalAsset();
	if (skeletalMetalAsset != nullptr)
	{
		AnimationPlayer* animationPlayer = new AnimationPlayer(skeletalModelComponent, skeletalMetalAsset->GetBoneAsset());
		skeletalModelComponent->SetAnimationPlayer(animationPlayer);
		animationPlayer->InitAnimationPlayer(m_deviceCached);
	}
}

void ComponentInitializer::Visit(CameraComponent* cameraComponent)
{
	InitBaseComponent(cameraComponent);
	cameraComponent->UpdateViewEntity();
	cameraComponent->UpdateBoundingProperty();

	DynamicBuffer& viewProjMatrixBuffer = cameraComponent->GetViewEntityBuffer();
	viewProjMatrixBuffer.InitializeBuffer(m_deviceCached);

	auto& film = cameraComponent->GetFilm();
	auto& idFilm = cameraComponent->GetIDFilm();
	auto& idStagingFilm = cameraComponent->GetIDStatgingFilm();
	auto& depthStencilView = cameraComponent->GetDepthStencilView();

	film.InitializeByOption(m_deviceCached, m_deviceContextCached);
	idFilm.InitializeByOption(m_deviceCached, m_deviceContextCached);
	idStagingFilm.InitializeByOption(m_deviceCached, m_deviceContextCached);
	depthStencilView.InitializeByOption(m_deviceCached, m_deviceContextCached);
}

void ComponentInitializer::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
	InitBaseComponent(sphereCollisionComponent);
	sphereCollisionComponent->UpdateBoundingProperty();
	sphereCollisionComponent->UpdateBoundingVolumeHierarchy();
}

void ComponentInitializer::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	InitBaseComponent(orientedBoxCollisionComponent);
	orientedBoxCollisionComponent->UpdateBoundingProperty();
	orientedBoxCollisionComponent->UpdateBoundingVolumeHierarchy();
}

void ComponentInitializer::Visit(SpotLightComponent* spotLightComponent)
{
	InitBaseComponent(spotLightComponent);
	spotLightComponent->UpdateViewEntity();
	spotLightComponent->UpdateBoundingProperty();
}

void ComponentInitializer::Visit(PointLightComponent* pointLightComponent)
{
	InitBaseComponent(pointLightComponent);
	pointLightComponent->UpdatePointLightFrustums();


	for (size_t idx = 0; idx < 6; ++idx)
	{
		PointLightFrustum& pointLightFrustum = pointLightComponent->GetPointLightFrustum(idx);
		DynamicBuffer& viewEntityBuffer = pointLightFrustum.GetViewEntityBuffer();
		viewEntityBuffer.InitializeBuffer(m_deviceCached);
	}
}

void ComponentInitializer::InitBaseComponent(AComponent* component)
{
	component->UpdateEntity();

	ConstantBuffer& componentEntityBuffer = component->GetComponentEntityBuffer();
	DynamicBuffer& transformationEntityBuffer = component->GetTransformationEntityBuffer();

	componentEntityBuffer.InitializeBuffer(m_deviceCached);
	transformationEntityBuffer.InitializeBuffer(m_deviceCached);
}